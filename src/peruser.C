#include <rpol.h>
#include <iostream>

int main()
{
	Regex A("((a|b)|(caa(aa))|c)*"_re);
	A._print();

	return 0;
}
