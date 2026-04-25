#ifndef __TEST_VALID_SYMBOLS_H__
#define __TEST_VALID_SYMBOLS_H__

#include "_util.h"
#include <rpol.h>
#include <concepts>
#include <cstdbool>

namespace {

static constexpr const Literal all_valid_symbols = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

template <Literal S=all_valid_symbols, bool Is=true>
void check_str()
{	
	static_assert(requires { requires is_regex<S> == Is; }, "❌ Invalid symbol.");
}

}

#endif /* __TEST_VALID_SYMBOLS_H__ */
