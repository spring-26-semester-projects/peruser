#ifndef __RPOL__
#define __RPOL__

#include <string>
#include <algorithm>
#include <concepts>

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
		for (const char s : S.data) {
			if (s == '\0') break;

			if (s == ' ') continue;

			if (!((s >= 'a' && s <= 'z') || (s >= 'A' && s <= 'Z') || (s >= '0' && s <= '9'))) {
				bool tmp = false;
				switch (s) {
					case '*':
					case '+':
					case '?':
					case '|':
						tmp = true;
						break;
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

template <Literal S> 
constexpr auto operator"" _re() requires is_regex_v<S>
{
	return Is_Regex<S>{ };
}

struct Regex {
	/*
		There is a compile-time check for ‘‘valid symbols’’ (including the emtpy string),
		otherwise: the class takes a valid regular expression, and stores it for persistent view.
		
		A mutable view ‘‘_m’’ exists, but it should not be accessed for correctness-sake
		even inside the class itself aside from constructors, and operators.
	*/

	std::string _m { };
	std::string_view expr { };
	std::string_view::const_iterator it { };

	void torpol();

	Regex() = default;
	
	template <Literal S>
	Regex(Is_Regex<S>&&) : _m(S.data)
	{
		torpol();

		expr = _m;
		it  = expr.cbegin();
	}	

	Regex& operator=(Regex& fregex) = delete;

	Regex(Regex&&) noexcept;

	Regex& operator=(Regex&&) noexcept;

	bool operator==(const Regex&);

	#ifdef __DEBUG_BUILD
	void _print() const;
	#endif
};

#endif /* __RPOL__ */
