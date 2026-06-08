#include <rpol.h>
#include <iostream>

int main()
{
//	std::string str;

//	std::getline(std::cin, str);
	Regex A("abb"_re);	
	//Regex A("(a(a|(abb|bb(a|b|c)a)a)a)"_re);
//		"aa.abb.bba.b.c.||a.|a.|a."
// 		"(a(a(abbbb(ab||c|)a)a)a)|"
//		"aaXaa(a|b|)b)"
	A._print();

	return 0;
}
