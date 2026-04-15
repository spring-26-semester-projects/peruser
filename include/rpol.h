#ifndef __RPOL__
#define __RPOL__

#include <cstddef>
#include <cstdbool>
#include <string>
#include <algorithm>
#include <concepts>

namespace {
std::string_view torpol(std::string in)
{
	const std::string_view L("*+?");

	int Rc = 0;
	for (auto rit = in.rbegin(); rit < in.rend()-1; ++rit) {
		std::for_each(L.cbegin(), L.cend(), [rit,&Rc](const char R) {
			if (*rit == R) Rc = 2;
		});

		if (*rit != '|') Rc++;
		
		if (*rit == '|' && Rc > 1) {
			char tmp = *rit;

			*rit = *(rit-1);
			*(rit-1) = *(rit-2);
			*(rit-2) = tmp;

			Rc = 0;
		} else if (*rit == '|') {
			std::swap(*rit, *(rit-1));
		}
	}

	return std::string_view(in);
};

template <std::size_t N>
struct Literal
{
	static constexpr std::size_t size = N;
	char data[N] { };

	consteval Literal(const char (&s)[N])
	{
		std::copy(s, s + N, &data[0]);
	}
};

template <Literal S>
concept is_regex_v = requires {
	requires []() constexpr {
		const std::string_view R("*+?|");

		for (const char s : S.data) {
			if (s == '\0') break;

			if (s > 122) {
				bool tmp = false;
				for (const char r : R) {
					if (s == r) tmp = true;
				}

				if (!tmp) return false;
			}
		}

		return true;
	}();
};

template <Literal S>
struct Is_Regex {
	static constexpr auto value = S;
};

struct Regex {
	std::string _m { };
 	std::string_view expr { };
	std::string_view::const_iterator it { };

	Regex() = default;

	template <Literal S>
	Regex(Is_Regex<S>&&) : _m(torpol(std::string(S.data)))
	{
		expr = _m;
		it   = expr.cbegin();
	}	

	Regex& operator=(Regex& fregex) = delete;

	Regex(Regex&& fregex) noexcept : _m(std::move(fregex._m))
	{
		this->expr = std::string_view(this->_m);
		this->it   = this->expr.cbegin() + std::distance(fregex.expr.cbegin(), fregex.it);

		fregex.expr = { };
	}

	Regex& operator=(Regex&& fregex) noexcept
	{
		if (this != &fregex) {
			this->_m   = std::move(fregex._m);
			this->expr = std::string_view(this->_m);
			this->it   = this->expr.cbegin() + std::distance(fregex.expr.cbegin(), fregex.it);

			fregex.expr = { };
		}

		return *this;
	}

	#ifdef __DEBUG_BUILD
	#include <iostream>
	void print() const { std::cout << expr << '\n'; }
	#endif
};

template <Literal S> requires is_regex_v<S>
constexpr auto operator"" _re()
{
	return ::Is_Regex<S>{ };
}

}
#endif /* __RPOL__ */
