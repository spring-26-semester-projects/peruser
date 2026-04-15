#include <iostream>
#include <rpol.h>

int main()
{
	// just a test
	Regex A("a|b*|c*|ab|c"_re);

	A.print();

	return 0;
}
