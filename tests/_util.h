#ifndef __TEST__UTIL_H__
#define __TEST__UTIL_H__

#include <rpol.h>
#include <type_traits>
#include <memory>

struct Ugrope {
	std::size_t size;
	std::size_t offset; 
};

template <Ugrope... U>
struct Groper {
	const void *data { };
	static constexpr Ugrope sv[] = { 
		U... 
	};
	
	const void *operator[](std::size_t idx) const
	{
		if (idx >= sizeof...(U)) throw std::out_of_range("Invalid Range for operator[].");

		return static_cast<const unsigned char*>(data) + sv[idx].offset;
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

	union { // to those with eyes that can C
		// this type is for thee
		Regex R;
		unsigned char rs[sizeof(Regex)];
	};

	using GV = Groper<U...>;

	Groper_view() = default;

	Groper_view(Regex&& fregex, std::unique_ptr<Regex>& fptr)
	{
		fptr = std::make_unique<Regex>(std::move(fregex));
	}

	Groper_view(const Groper_view&) = delete;
	Groper_view& operator=(const Groper_view&) = delete;

	const unsigned char peek(std::size_t obj, std::size_t idx, const std::unique_ptr<Regex>& fptr) const {
		return static_cast<const unsigned char*>(GV{ fptr.get() }[obj])[idx];
	}

	~Groper_view() { }
};

#endif /* __TEST__UTIL_H__ */
