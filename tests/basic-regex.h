#ifndef __TEST_BASIC_REGEX_H__
#define __TEST_BASIC_REGEX_H__

#include "_util.h"
#include <rpol.h>
#include <cstddef>
#include <cstdbool>
#include <utility>
#include <cctype>
#include <memory>
#include <cstring>
#include <cassert>

template <Literal S>
void reverse_polish_test(unsigned char buf[sizeof(S)])
{
	/* 
		Checks if string is in memory, if not,
		it goes to the location in memory, and loops over string.

		It returns the string itself as a C-string buffer.
	*/

	static constexpr const bool is_long = (sizeof(S)-1) > 15;
	static constexpr const std::size_t s = sizeof(std::declval<Regex>()._m), offs = __builtin_offsetof(Regex, _m);

	using Rview_raw = ::Groper_view<Ugrope{s,offs}>;
	std::unique_ptr<Regex> R;
	Rview_raw RV(Regex(operator""_re<S>()), R);

	#if defined(__GNUC__)
	if constexpr (is_long) {
			
	uintptr_t str { };
	std::size_t size { };
	unsigned char ubuf[8];

	for (std::size_t i = 0; i < 8; ++i) ubuf[i] = RV.peek(0,i,R);
	std::memcpy(&str, ubuf, 8);
	
	for (std::size_t i = 0; i < 8; ++i) ubuf[i] = RV.peek(0,i+8,R);
	std::memcpy(&size, ubuf, 8);

	unsigned char fbuf[size];
	unsigned char *ustr = reinterpret_cast<unsigned char*>(str);
	for (std::size_t i = 0; i < size; ++i) fbuf[i] = ustr[i];
	
	std::memcpy(buf, fbuf, sizeof(S));

	} else {
	#endif

	if (is_long) throw std::runtime_error("Long string comparsions are forbidden without GCC.");
	
	unsigned char ubuf[s];

	std::size_t j = 0, k = 0;
	unsigned char i = RV.peek(0, j, R);
	for (;j < s-1;) {
		bool tmp = false;

		switch (i) {
			case '*':
			case '|':
			case '?':
				tmp = true;
				break;
		}

		if (std::isalpha(i) || std::isdigit(i) || tmp) { 
			ubuf[k] = i;
			++k;
		}
		
		i = RV.peek(0,++j,R);
	}

	std::memcpy(buf, ubuf+(k-sizeof(S)+1), sizeof(S)-1);
	#if defined(__GNUC__)
	}
	#endif
}

template <Literal S>
void check_regex(const char *rs)
{
	unsigned char buf[sizeof(S)];

	reverse_polish_test<S>(buf);
	
	assert(std::strcmp(reinterpret_cast<const char*>(buf),rs) == 0);
}

#endif /* __TEST_BASIC_REGEX_H__ */
