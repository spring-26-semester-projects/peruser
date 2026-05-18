#include <ratomic.h>
#include <rpol.h>

#include <cassert>
#include <iostream>

static Q dummy_delta(Q states, Regex&& r)
{
    (void)r;
    return states;
}

static void test_literal_nfa()
{
    Regex A("a"_re);
    std::string_view target_view = A.expr;

    NFA nfa(Q(), Q(), target_view, 0, dummy_delta);
    nfa.read_tape(std::move(A));

    assert(nfa.Qm.q.size() == 1 && nfa.Qm.q[0] == 0);
    assert(nfa.Fm.q.size() == 1 && nfa.Fm.q[0] == 1);
    assert(nfa.transitions.size() == 1);
    assert(nfa.transitions[0].symbol == 'a');
    std::cout << "[PASS] Literal Node NFA Generation\n";
}

static void test_alternation_nfa()
{
    Regex A("a|b"_re);
    std::string_view target_view = A.expr;

    NFA nfa(Q(), Q(), target_view, 0, dummy_delta);
    nfa.read_tape(std::move(A));

    assert(nfa.Qm.q[0] == 4);
    assert(nfa.Fm.q[0] == 5);

    int epsilon_branches = 0;
    for (const auto& t : nfa.transitions) {
        if (t.from == 4 && t.symbol == '\0') epsilon_branches++;
    }
    assert(epsilon_branches == 2);
    std::cout << "[PASS] Alternation Structure Epsilon-Envelope\n";
}

static void test_kleene_star_nfa()
{
    Regex A("a*"_re);
    std::string_view target_view = A.expr;

    NFA nfa(Q(), Q(), target_view, 0, dummy_delta);
    nfa.read_tape(std::move(A));

    assert(nfa.Qm.q[0] == 2);
    assert(nfa.Fm.q[0] == 3);
    std::cout << "[PASS] Kleene Star Sub-Graph Looping\n";
}

int main()
{
    std::cout << "Executing ToCC Core Test Harness...\n";
    std::cout << "-----------------------------------\n";

    test_literal_nfa();
    test_alternation_nfa();
    test_kleene_star_nfa();

    std::cout << "-----------------------------------\n";
    std::cout << "All core Thompson validation checks PASSED.\n";
    return 0;
}