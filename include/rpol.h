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
#include <cassert>
#include <cstdbool>

#include <iostream>

#define REGEX_MAX_LEN INT_MAX

template <std::size_t N=1>
struct Literal
{
	static constexpr std::size_t size = N;
	char data[N] { };

	constexpr Literal() = default;

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

			if (s == ' ') continue;

			switch (s) {
				case '*':
				case '?':
				case '+':
				case '|':
					escape = false;
					continue;
			}

			if (escape) return false;

			switch (s) {
				case '(':
					count++;
					continue;
				case ')':
					count--;
					continue;	
				case '\\':
					escape = true;
					continue;
			}

			if (!((s >= 'a' && s <= 'z') || (s >= 'A' && s <= 'Z') || (s >= '0' && s <= '9'))) return false;
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
constexpr auto operator""_re()
{
	return Is_Regex<S>{};
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

	std::string torpol(std::string&);

	Regex() = default;

	template <Literal S>
	Regex(Is_Regex<S>&&)
	{
		static constexpr auto tmp = torpol<S>();
		_m.assign(tmp.data, S.size);

		expr = _m;
		it  = expr.cbegin();
	}

	Regex(std::string& fstr)
	{
		auto tmp = torpol(fstr);
		_m.assign(tmp.data(), tmp.size());

		expr = _m;
		it   = expr.cbegin();
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

	constexpr std::size_t size() const
	{
		return static_cast<std::size_t>(top-base);
	}
	
	constexpr auto operator[](std::size_t s) const
	{
		if constexpr (std::is_same_v<R, Regex>) {
			if (top != base && s < base->size()) {
				return (*(top-1))[s];
			}

			return char{};
		} else {
			if (s < static_cast<std::size_t>(top - base)) {
				return *(base+s);
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

constexpr bool atom_begin(const char c)
{
        switch (c) {
                case 'G':
                case 'Q':
                        return true;
                default:
                        return false;
        }
}

constexpr bool atom_end(const char c)
{
        switch (c) {
                case 'G':
                case 'P':
                case 'R':
                        return true;
                default:
                        return false;
        }
}

constexpr int precedence(const char c)
{
        switch (c) {
                case '|':
                        return 1;
                case '.':
                        return 2;
                case '*':
                case '?':
                case '+':
                        return 3;
                default:
                        return 0;
        }
}

constexpr char symb(const char c, bool &escp)
{
        if (escp) {
                escp = false;
                return 'G';
        }

        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
                return 'G';
        }

        switch (c) {
                case ' ':
                        return ' ';
                case '\\':
                        escp = true;
                        return '\0';
                case ')':
                        return 'P';
                case '(':
                        return 'Q';
                case '*':
                case '?':
                case '+':
                        return 'R';
                case '|':
                        return 'T';
                default:
                        return '\0';
        }
}

template <Literal S>
constexpr auto Regex::torpol()
{
        /*
                torpol is an infix-to-reverse-polish notation converter that assumes CORRECT SYNTAX.

                If the syntax is incorrect, e.g., "|a", then it will produce UB.
                torpol should not be used outside the regex class hence...
        */

        Literal<S.size * 2> q {};
        Stackbuf<char, S.size * 2> rs;

        std::size_t i = 0;
        bool escp = false;
        char prev = '\0';

        for (auto it = std::begin(S.data); it != std::end(S.data) && *it != '\0'; ++it) {
                if (*it == ' ') continue;

                char cur = symb(*it, escp);
                if (cur == '\0') continue;

                if (prev != '\0' && atom_end(prev) && atom_begin(cur)) {
			while (rs.size() > 0 && precedence(rs.peek()) >= precedence('.')) {
                                q.data[i++] = rs.peek();
                                rs.pop();
                        }

                        rs.push('.');
                }

                switch (cur) {
                        case 'G':
                                q.data[i++] = *it;
                                break;

                        case 'Q':
                                rs.push('(');
                                break;

                        case 'P':
                                while (rs.size() > 0 && rs.peek() != '(') {
                                        q.data[i++] = rs.peek();
                                        rs.pop();
                                }

                                while (rs.size() > 0 && rs.peek() == '(') rs.pop();
                                break;

                        case 'R':
                        case 'T':
                                while (rs.size() > 0 && precedence(rs.peek()) >= precedence(*it)) {
                                        q.data[i++] = rs.peek();
                                        rs.pop();
                                }

                                rs.push(*it);
                                break;
                }

                prev = cur;
        }

        while (rs.size() > 0) {
                char tmp = rs.peek();
                if (!(tmp == ')' || tmp == '(')) q.data[i++] = tmp;
                rs.pop();
        }

        q.data[i] = '\0';
        return q;
}

#endif /* __RPOL__ */
