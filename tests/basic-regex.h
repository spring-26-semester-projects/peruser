#ifndef __TEST_BASIC_REGEX_H__
#define __TEST_BASIC_REGEX_H__

#include "_util.h"
#include <rpol.h>
#include <cstddef>
#include <utility>

template <Literal S, Literal W>
void check_regex()
{
	static constexpr const std::size_t size = sizeof(std::declval<Regex>()._m), offs = __builtin_offsetof(Regex, _m);

	using Rview_cmp   = ::Groper_view<Ugrope{size,offs}>;
	Rview_cmp RV(Regex(operator""_re<S>()));

	RV.template regex_cmp<S,W>();
}

#endif /* __TEST_BASIC_REGEX_H__ */
