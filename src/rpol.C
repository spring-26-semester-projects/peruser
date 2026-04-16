#include <rpol.h>

// TODO: add stack to support use of parantheses.
void Regex::torpol()
{
	/*
		torpol is an infix-to-reverse-polish notation converter that assumes CORRECT SYNTAX.

		If the syntax is incorrect, e.g., "|a", then it will produce UB.
		torpol should not be used outside the regex class hence...

		it checks if aRb, if so, abR, else
		either aR, where R = *, +, ?, and if so aR
		if neither, then ab, bb, etc., then (bb) = bR and (aa) = aR, but not (aaa) = aaR.
	*/

	int Rc = 0;
	for (auto rit = _m.rbegin(); rit < _m.rend()-1; ++rit) {
		if (*rit == ' ') continue;

		switch (*rit) {
			case '*':
			case '+':
			case '?':
				Rc = 2;
				break;
		}

		if (*rit != '|') Rc++;
		
		if (*rit == '|' && Rc > 1) {
			std::rotate(rit.base()-1, rit.base(), rit.base()+2);

			Rc = 0;
		} else if (*rit == '|') {
			std::swap(*rit, *(rit-1));
		}
	}
}

Regex::Regex(Regex&& fregex) noexcept : _m(std::move(fregex._m))
{
	this->expr = std::string_view(this->_m);
	this->it   = this->expr.cbegin() + std::distance(fregex.expr.cbegin(), fregex.it);

	fregex.expr = { };
}

Regex& Regex::operator=(Regex&& fregex) noexcept
{
	if (this != &fregex) {
		this->_m   = std::move(fregex._m);
		this->expr = std::string_view(this->_m);
		this->it   = this->expr.cbegin() + std::distance(fregex.expr.cbegin(), fregex.it);

		fregex.expr = { };
	}

	return *this;
}

bool Regex::operator==(const Regex& fregex)
{
	for (auto it = this->it; it != this->expr.cend(); ++it) {
		if (*(fregex.expr.cbegin() + std::distance(this->expr.cbegin(), it)) != *it) {
			return false;
		}
	}

	return true;
}

#ifdef __DEBUG_BUILD
#include <iostream>
void Regex::_print() const { std::cout << expr << '\n'; }
#endif

