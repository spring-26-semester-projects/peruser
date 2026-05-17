#ifndef __RPOL__
#define __RPOL__

#include <string>
#include <stdexcept>
#include <algorithm>
#include <concepts>
#include <type_traits>
#include <utility>
#include <climits>
#include <iterator>

#include <iostream>

#define REGEX_MAX_LEN INT_MAX

template <std::size_t N>
struct Literal
{
	static constexpr std::size_t size = N;
	char data[N] { };

	consteval Literal(const char (&s)[N])
	{
		std::copy(s, s+N, &data[0]);
	}

	constexpr Literal(const char *base, const char *top)
	{
		std::reverse_copy(base, top, &data[0]);
	}
};

template <Literal S>
concept is_regex = requires {
	requires []() constexpr {
		if (S.size > REGEX_MAX_LEN) return false;

		int count   = 0;
		bool escape = false;
		for (const char s : S.data) {
			if (s == '\0') break;

			switch (s) {
				case ' ':
					continue;
				case '(':
					count++;
					continue;
				case ')':
					count--;
					continue;
				case '\\':
					escape = true;
					continue;
				default:
					break;
			}

			if (!((s >= 'a' && s <= 'z') || (s >= 'A' && s <= 'Z') || (s >= '0' && s <= '9')) || escape) {
				bool tmp = false;
				switch (s) {
					case '*':
					case '?':
					case '|':
						tmp = true;
						break;
				}

				if (!tmp) return false;
				if (escape) escape = false;
			}
		}

		if (count != 0) return false;

		return true;
	}();
};

template <Literal S>
struct Is_Regex {
	static constexpr auto value = S;
};

template <Literal S> requires is_regex<S>
constexpr auto operator"" _re()
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

	template <Literal S>
	static constexpr auto torpol();

	Regex() = default;
	
	template <Literal S>
	Regex(Is_Regex<S>&&)
	{
		static constexpr auto tmp = torpol<S>();
		_m.assign(tmp.data, S.size);
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

template <typename R, std::size_t N>
struct Stackbuf {
	static constexpr const std::size_t capacity { N };

	using T = std::conditional_t<std::is_same_v<R,Regex>, decltype(std::declval<Regex>().expr), R>;

	T _buf[capacity];

	T *base { nullptr }, *top { nullptr };

	constexpr Stackbuf() : base(_buf), top(_buf) { }

	constexpr void push(T c)
	{
		if (!isFull()) {
			*top++ = c;
			
			return;	
		}

		throw std::overflow_error("Stack overflow.");
	}

	constexpr void push(Regex r)
	{
		if constexpr (std::is_same_v<R,Regex>) {
			if (!isFull()) {
				*top++ = r.expr;
				
				return;	
			}

			throw std::overflow_error("Stack overflow.");
		}
	}

	constexpr void pop()
	{
		if (top != base) --top;
	}

	constexpr bool isFull() const
	{
		return static_cast<std::size_t>(top-base) >= capacity;
	}

	constexpr auto operator[](std::size_t s) const
	{
		if constexpr (std::is_same_v<R, Regex>) {
			if (top != base && s < base->size()) {
				return (*(top - 1))[s];
			}

			return char{};
		} else {
			if (s < static_cast<std::size_t>(top - base)) {
				return *(base + s);
			}

			return T{};
		}
	}

	constexpr T peek() const
	{
		if (top != base) return *(top-1);

		return T{};
	}

	constexpr auto pop_stack() const
	{
		return Literal<N>(base, top);
	}
};

template <Literal S>
constexpr auto Regex::torpol()
{
	/*
		torpol is an infix-to-reverse-polish notation converter that assumes CORRECT SYNTAX.

		If the syntax is incorrect, e.g., "|a", then it will produce UB.
		torpol should not be used outside the regex class hence...

		A  = (ab*c|d...)
		T  = operator : |, *, (, ), ?
		Nc = |A,'()'|, i.e., nesting count.
		Lc = |A|-(Nc*2), i.e., size of expression without aux. symbols, or length count.
		Rc = last nested level reading.

		if str = ATB then
			swap(T,B)

		swap X Y:
			shift_right(X, Lc+(Nc*2))
		

		(The implicit ab. meaning concatenate(a,b) and groupings are left for convenience.)
	*/

	Stackbuf<char,S.size> rs;

	auto start = std::rbegin(S.data)+1, end = std::rend(S.data);

	int Rc = 0, Lc = 0, Nc = 0;
	for (auto rit = start; rit != end; ++rit) {
		if (*rit == ' ') continue;

		rs.push(*rit);

		Rc = Nc;

		if (*rit == ')') {
			Nc++;

			Lc = 0;
		} else if (*rit == '(') {
			Nc--;
		}

		if (*rit != '|') Lc++;

		if (*rit == '|') {
			if ((Lc-Nc) > 1) {
				std::rotate(rs.top-Lc-(Nc<<1), rs.top-1, rs.top);
			} else {
				std::swap(*(rs.top-2),*(rs.top-1));
			}

			if (Rc == Nc) Lc = 0;
		}
	}
	
	return rs.pop_stack();
}

#endif /* __RPOL__ */
