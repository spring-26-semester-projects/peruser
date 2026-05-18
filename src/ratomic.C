#include <stdexcept>
#include <string_view>

#include "ratomic.h"

void NFA::read_tape(Regex&& tape)
{
    NFAFragment stack_memory[32]{};
    Stack<32> fragment_stack(stack_memory);

    unsigned char state_counter = 0;
    auto next_state = [&state_counter]() { return state_counter++; };

    std::string_view postfix = tape.expr;
    transitions.clear();

    for (char symbol : postfix) {
        if (symbol == ' ') continue;

        if ((symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z') ||
            (symbol >= '0' && symbol <= '9')) {
            unsigned char s = next_state();
            unsigned char e = next_state();

            transitions.push_back({s, symbol, e});
            fragment_stack.push(NFAFragment{s, e});
        }
        else if (symbol == '|') {
            NFAFragment r2{};
            NFAFragment r1{};
            fragment_stack.pop(r2);
            fragment_stack.pop(r1);

            unsigned char new_start = next_state();
            unsigned char new_accept = next_state();

            transitions.push_back({new_start, '\0', r1.start_state});
            transitions.push_back({new_start, '\0', r2.start_state});
            transitions.push_back({r1.accept_state, '\0', new_accept});
            transitions.push_back({r2.accept_state, '\0', new_accept});

            fragment_stack.push(NFAFragment{new_start, new_accept});
        }
        else if (symbol == '*') {
            NFAFragment r{};
            fragment_stack.pop(r);

            unsigned char new_start = next_state();
            unsigned char new_accept = next_state();

            transitions.push_back({new_start, '\0', r.start_state});
            transitions.push_back({new_start, '\0', new_accept});
            transitions.push_back({r.accept_state, '\0', r.start_state});
            transitions.push_back({r.accept_state, '\0', new_accept});

            fragment_stack.push(NFAFragment{new_start, new_accept});
        }
    }

    NFAFragment final_nfa{};
    fragment_stack.pop(final_nfa);

    this->Qm.q = std::vector<unsigned char>{final_nfa.start_state};
    this->Fm.q = std::vector<unsigned char>{final_nfa.accept_state};
}