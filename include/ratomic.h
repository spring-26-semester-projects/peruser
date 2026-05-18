#ifndef __RATOMIC__
#define __RATOMIC__

#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <vector>

#include "rpol.h"

template <typename>
struct is_std_vector : std::false_type {};

template <typename T, typename U>
struct is_std_vector<std::vector<T, U>> : std::true_type {};

template <typename T>
concept is_std_vector_v = requires { is_std_vector<T>::value; };

struct Q {
    std::vector<unsigned char> q{};

    Q() = default;
    Q(Q&&) noexcept = default;
    Q(const Q&) = default;

    Q(const std::vector<unsigned char>& fq) : q(fq) {}
    Q(std::vector<unsigned char>&& fq) noexcept : q(std::move(fq)) {}

    template <typename T>
        requires is_std_vector_v<T>
    Q(T fq) : q(fq)
    {
    }

    template <typename T>
    Q(T) = delete;

    Q& operator=(const Q&) = delete;
    Q& operator=(Q&& fQ) noexcept
    {
        if (this != &fQ) this->q = std::move(fQ.q);

        return *this;
    }
};

typedef Q (*Delta)(Q, Regex&&);

struct TransitionStep {
    unsigned char from;
    char symbol;
    unsigned char to;
};

struct NFA {
    Q Qm{};
    Q Fm{};
    const std::string_view L{};
    const unsigned char q0{0};
    Delta Dm{nullptr};
    std::vector<TransitionStep> transitions{};

    NFA(Q fQm, Q fFm, std::string_view fL, unsigned char fq0, Delta fDm)
        : Qm(std::move(fQm)), Fm(std::move(fFm)), L(fL), q0(fq0), Dm(fDm)
    {
    }

    void read_tape(Regex&& tape);
};

struct NFAFragment {
    unsigned char start_state;
    unsigned char accept_state;
};

template <std::size_t N = 16>
struct Stack {
    std::size_t capacity = N;
    NFAFragment* const base = nullptr;
    NFAFragment* top = nullptr;

    Stack(NFAFragment* const fbase) : base(fbase), top(fbase) {}

    void push(NFAFragment&& frag)
    {
        if (static_cast<std::size_t>(top - base) >= capacity) {
            throw std::runtime_error("Stack overflow during NFA structural build.");
        }
        *top = std::move(frag);
        ++top;
    }

    void pop(NFAFragment& frag)
    {
        if (top == base) {
            throw std::runtime_error("Stack underflow during NFA structural build.");
        }
        --top;
        frag = std::move(*top);
    }

    ~Stack() = default;
};

#endif /* __RATOMIC__ */
