#include <iostream>
#include <rpol.h>

int main()
{
	// just a test
	std::string R = { "a|b*|c*|ab|c" };
	Regex A(R);

	std::cout << '\n';

	return 0;
}
