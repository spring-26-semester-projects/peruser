#ifndef __RATOMIC__
#define __RATOMIC__

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

    template <typename T>
        requires is_std_vector_v<T>
    Q(T fq) : q(fq)
    {
    }

    template <typename T>
    Q(T) = delete;

    template <typename T>
        requires is_std_vector_v<T>
    Q(T&& fq) noexcept : q(std::move(fq))
    {
    }

    Q& operator=(const Q&) = delete;
    Q& operator=(Q&& fQ) noexcept
    {
        if (this != &fQ) this->q = std::move(fQ.q);

        return *this;
    }
};

typedef Q (&Delta)(Q, Regex&&);

struct Nfa {
    Q Qm;
    Q Fm;

    const std::string_view L{};
    const unsigned char q0{};
    Delta Dm;

    void read_tape(Regex);
};

template <std::size_t N = 2>
struct Stack {
    std::size_t capacity = N;
    Regex *const base = nullptr, *top = nullptr;

    Stack(Regex* const fbase) : base(fbase) { top = base; }

    Stack(Stack&&);

    Stack& operator=(const Stack&) = delete;

    Stack& operator=(Stack&&);

    void resize(std::size_t) const;

    void push(Regex&& r);
    void pop(Regex&& r);

    void peek() const;

    ~Stack();
};

#endif /* __RATOMIC__ */
