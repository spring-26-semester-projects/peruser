/**
 * Implemented with &love; by 0xq4b4s.
 */
#include <rpol.h>
#include <stack>

void Regex::torpol()
{
    std::string output;
    std::stack<char> ops;

    auto precedence = [](char op) {
        return (op == '|') ? 1 : 2;
    };

    for (char c : _m) {
        if (c == ' ') continue;

        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
            output += c;
        } else if (c == '(') {
            ops.push(c);
        } else if (c == ')') {
            while (!ops.empty() && ops.top() != '(') {
                output += ops.top();
                ops.pop();
            }
            if (!ops.empty()) ops.pop();
        } else {
            int prec = precedence(c);
            while (!ops.empty() && ops.top() != '(' && precedence(ops.top()) >= prec) {
                output += ops.top();
                ops.pop();
            }
            ops.push(c);
        }
    }

    while (!ops.empty()) {
        output += ops.top();
        ops.pop();
    }

    _m = output;
}

Regex::Regex(Regex &&fregex) noexcept
{
    std::size_t dist = std::distance(fregex.expr.cbegin(), fregex.it);

    this->_m = std::move(fregex._m);
    this->expr = std::string_view(this->_m);
    this->it = this->expr.cbegin() + dist;

    fregex.expr = {};
}

Regex &Regex::operator=(Regex &&fregex) noexcept
{
    if (this != &fregex)
    {
        std::size_t dist = std::distance(fregex.expr.cbegin(), fregex.it);

        this->_m = std::move(fregex._m);
        this->expr = std::string_view(this->_m);
        this->it = this->expr.cbegin() + dist;

        fregex.expr = {};
    }

    return *this;
}

bool Regex::operator==(const Regex &fregex)
{
    return this->expr == fregex.expr;
}

#ifdef __DEBUG_BUILD
#include <iostream>
void Regex::_print() const { std::cout << expr << '\n'; }
#endif