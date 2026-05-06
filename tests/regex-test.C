#include "valid-symbols.h"
#include "basic-regex.h"
#include <cassert>
#include <cstring>

void main_check_test()
{
	/** pass */
	check_str();

	/** pass */
	check_str<"a*|b|c">();

	/** fail,  */
	check_str<"Created with &love; by 0xq4b4s", false>();

	/** pass */
	check_str<" ">();

	/**
	 * fail, contains invalid symbol $.
	 */
	check_str<" hell $ o?", false>();

	check_str<"aaaaaaaaaaaaaaaaa ** bbbbbbbbbbbbbbbbbbbbbbb ** ccccccc">();
}

void main_polish_test()
{
	check_regex<"a|b", "ab|">();

	check_regex<"aaaaa|bb", "aaaaabb|">();

	check_regex<"slfdkjdlfk*jad*sgklasdjasaldlsaj*dhsjahd*k*sag*dyqwbdg", "slfdkjdlfk*jad*sgklasdjasaldlsaj*dhsjahd*k*sag*dyqwbdg">();

	check_regex<"a*|b|c|d|e|ff", "a*b|c|d|e|ff|">();

	// TODO: Fix this.
	// check_regex<"a  |   b  |   c*", "a     b  |   c*|">;
}

int main()
{
	main_polish_test();

	return 0;
}
