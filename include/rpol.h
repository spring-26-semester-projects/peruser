#ifndef __RPOL__
#define __RPOL__

#include <string>
#include <algorithm>
#include <utility>

const std::string_view L("*+?ab");

std::string_view torpol(std::string in)
{
	for (auto it = in.begin(); it != in.end(); ++it) {
		if (*it == '|') {
			if (std::next(it, 2) != in.end()) {
				char tmp = *it;
				std::for_each(L.cbegin(), L.cend(), [&it,tmp](char R) {
					if (*(it+2) == R) {
						*it = *(it+1);
						*(it+1) = *(it+2);
						*(it+2) = tmp;

						it += 1;
					}
				});
			} else {

				std::swap(*it, *(it+1));	
			}

			
			it += 1;
		} 
	}

	return std::string_view(in);
};

struct Regex {
	std::string_view expr { };
	std::string_view::const_iterator it { };

	explicit Regex(std::string fexpr) : expr(torpol(std::move(fexpr)))
	{	
		this->expr = fexpr;
		this->it   = this->expr.cbegin();
	}

	template<typename T>
	Regex(T) = delete;

	Regex() = delete;
	Regex& operator=(Regex& fregex) = delete;

	Regex& operator=(const Regex& fregex) noexcept
	{
		if (this != &fregex) {
			this->expr = fregex.expr;
			this->it   = this->expr.cbegin() + std::distance(fregex.expr.cbegin(), fregex.it);
		}

		return *this;
	}
};

#endif /* __RPOL__ */
