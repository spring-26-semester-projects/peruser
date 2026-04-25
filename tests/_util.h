#ifndef __TEST__UTIL_H__
#define __TEST__UTIL_H__

#include <rpol.h>
#include <cstring>
#include <stdexcept>
#include <cassert>

struct Ugrope {
	std::size_t size; // unused, but useful
	std::size_t offset; 
};

template <Ugrope... U>
struct Groper {
	const unsigned char *data { };
	static constexpr Ugrope sv[] = { 
		U... 
	};
	
	const unsigned char *operator[](std::size_t i) const
	{
		if (i >= sizeof...(U)) throw std::out_of_range("Invalid Range for operator[].");

		return data + sv[i].offset;
	}
};

template <Ugrope... U>
struct Groper_view {
	/*
		It takes a Regex instance and does not own its memory,
		and allows indexed access to the data byte-by-byte.

		It is useful when operator:: is not allowd for certain members.

		The Ugrope struct holds useful information about sizeof member, and offsetof member (relative to class).
	 */

	union _sv { // to those with eyes that can C
		// this type is for thee
		Regex R;
		const unsigned char rs[sizeof(Regex)];

		~_sv() { }
	} data;

	using GV = Groper<U...>;

	Groper_view() = default;

	Groper_view(Regex&& fregex) : data { .R = std::move(fregex) } { }

	Groper_view(const Groper_view&) = delete;
	Groper_view& operator=(const Groper_view&) = delete;

	const unsigned char peek(std::size_t obj, std::size_t idx) const
	{
		return GV{ data.rs }[obj][idx];
	}

	template <Literal S, Literal W>
	void regex_cmp();

	~Groper_view() { }
};

template <Ugrope... U>
template <Literal S, Literal W>
void Groper_view<U...>::regex_cmp()
{	
	static constexpr const std::size_t s = sizeof(S)-1;
	static constexpr const bool is_long  = s > 15;
	const unsigned char *start 	     = GV{data.rs}[0];

	#if defined(__GNUC__)
	if constexpr (is_long) {	
	
	uintptr_t buf { };
	std::memcpy(&buf, start, 8);

	assert(std::memcmp(W.data,reinterpret_cast<const char*>(buf),s) == 0);

	} else {
	#endif
	
	if constexpr (is_long) throw std::runtime_error("Long string comparsions are forbidden without GCC.");

	unsigned char i = peek(0, 0);
	for (std::size_t k : { 0, 1, 16, 32 }) { 
		switch (k) {
			case 0:
			case 1:
			case 16:
			case 32:
				i = peek(0,k);

				if (std::isalpha(i) || std::isdigit(i) || i == '*' || i == '|' || i == '?') {
					if (std::memcmp(W.data, start+k, s) == 0) return;
				}

				break;
		}
	}

	assert(false);

	#if defined(__GNUC__)
	}
	#endif
}

#endif /* __TEST__UTIL_H__ */
