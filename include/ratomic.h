#ifndef __RATOMIC__
#define __RATOMIC__

#include "rpol.h"
#include <vector>
#include <cstddef>
#include <type_traits>

#define MAX_VMSTACK_SIZE 1024

enum class Opcodes : unsigned char {
	SPLIT,
	JUMP,
	ACCEPT,
	TRAP,
	NEW,
};

template <typename>
struct is_std_vector : std::false_type { };

template <typename T, typename U>
struct is_std_vector<std::vector<T,U>> : std::true_type { };

template<typename T>
concept is_std_vector_v = requires { is_std_vector<T>::value; };

struct Q {
	std::vector<unsigned char> q { };
	
	template <typename T> requires is_std_vector_v<T>
	Q(T fq) : q(fq) { }

	template<typename T>
	Q(T) = delete;

	template <typename T> requires is_std_vector_v<T>
	Q(T&& fq) noexcept : q(std::move(fq)) { }

	Q& operator=(const Q&) = delete;
	Q& operator=(Q&& fQ) noexcept
	{
		if (this != &fQ) this->q = fQ.q;

		return *this;
	}
};

using Delta = unsigned char (*)(unsigned char, char);

struct Nfa {
	Q Qm;
	Q Fm;

	const std::string_view L { };
	const unsigned char q0 { };
	Delta Dm;
};

struct State {
	unsigned char qn { };
	char input { };
	unsigned char qt { };
};

struct VirtualMachine {
	Stackbuf<State, MAX_VMSTACK_SIZE> stack;
	Opcodes *code { };
	
	void read_tape(char *q)
	{
		code[1] = Opcodes::SPLIT;

		State tmp = { .input = *q, };
		stack.push(tmp);
	}
};

#endif /* __RATOMIC__ */
