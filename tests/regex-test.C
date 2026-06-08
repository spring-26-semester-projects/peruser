#include "valid-symbols.h"
#include "basic-regex.h"
#include <cassert>
#include <cstring>

void main_check_test()
{
	check_str(); // should pass.

	check_str<"a*|b|c">(); // should pass.
	
	check_str<"I met a traveller from an antique land who said, two vast and trunkless legs of stone, stand in the desert", false>(); // should fail.

	check_str<" ">(); // should pass.

	check_str<" hell $ o?", false>(); // should fail.

	check_str<"aaaaaaaaaaaaaaaaa ** bbbbbbbbbbbbbbbbbbbbbbb ** ccccccc">(); // should pass.

	check_str<" hell))))", false>(); // should fail.

	check_str<"()()()">(); // should pass.

	check_str<"\\*a\\|b\\?a\\|">(); // should pass.
}

void main_polish_test()
{
	check_regex<"a|b", "ab|">();

	check_regex<"aaaaa|bb","aaaaabb|">();

	check_regex<"slfdkjdlfk*jad*sgklasdjasaldlsaj*dhsjahd*k*sag*dyqwbdg", "slfdkjdlfk*jad*sgklasdjasaldlsaj*dhsjahd*k*sag*dyqwbdg">();

	check_regex<"a*|b|c|d|e|ff", "a*b|c|d|e|ff|">();

	check_regex<"a\\|b", "a\\b|">();

	// TODO: Fix this.
//	check_regex<"a  |   b  |   c*", "a     b  |   c*|">;
}

int main() {
	main_polish_test();

	return 0;
}
