#include <rpol.h>
#include <stdexcept>
#include <cstdbool>

#ifdef __DEBUG_BUILD
#include <iostream>
#endif

Regex::Regex(Regex&& fregex) noexcept
{
	std::size_t dist = std::distance(fregex.expr.cbegin(), fregex.it);

	this->_m = std::move(fregex._m);
	this->expr = std::string_view(this->_m);
	this->it   = this->expr.cbegin() + dist;

	fregex.expr = { };
}

Regex& Regex::operator=(Regex&& fregex) noexcept
{
	if (this != &fregex) {
		std::size_t dist = std::distance(fregex.expr.cbegin(), fregex.it);

		this->_m   = std::move(fregex._m);
		this->expr = std::string_view(this->_m);
		this->it   = this->expr.cbegin() + dist; 

		fregex.expr = { };
	}

	return *this;
}

bool Regex::operator==(const Regex& fregex)
{
	return this->expr == fregex.expr;
}

#ifdef __DEBUG_BUILD
void Regex::_print() const { std::cout << expr << '\n'; }
#endif

