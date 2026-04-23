#ifndef __TEST_VALID_SYMBOLS_H__
#define __TEST_VALID_SYMBOLS_H__

#include "_util.h"
#include <rpol.h>
#include <functional>
#include <concepts>

namespace {

static constexpr const Literal all_valid_symbols = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

template <Literal S=all_valid_symbols>
void check_str()
{	
	static_assert(requires { requires is_regex_v<S>; }, "❌ Invalid symbol.");
}

using Rview = ::Groper_view<Ugrope{sizeof(Regex), 0}>;

template <Literal S, typename F>
void grope_regex(F&& L) requires std::invocable<F, const Rview&>
{
	/*
		Use it to go over the entire Regex class by passing it a function/handler.
	*/

	std::unique_ptr<Regex> R;

	Rview gv(Regex(operator""_re<S>()), R);

	L(gv);
}

}

#endif /* __TEST_VALID_SYMBOLS_H__ */
