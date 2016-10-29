#include <iostream>
#include <stack>
#include <queue>
#include <set>
#include "common.h"
#include "RE2NFA.h"
#include "NFA2DFA.h"

using namespace std;

set<char> path_set;

int main() {
    string re;
    cin >> re;
    for (int i = 0; i < re.length(); ++i) {
        if (isChar(re[i])) path_set.insert(re[i]);
    }
    RE2NFA* nfa = new RE2NFA();
    nfa->addDotToRE(re);
    cout << "-------Formal RE--------" << endl;
    cout << re << endl;
    nfa->RE2POST(re);
    cout << "-------Postfix RE--------" << endl;
    cout << re << endl;
    nfa->m_RE2NFA(re);
    cout << "-------NFA--------" << endl;
    printAllStates(nfa->NFA_states, nfa->state_nums, nfa->NFA_e, false);
    NFA2DFA* dfa = new NFA2DFA(nfa->NFA_states, nfa->NFA_e, path_set, nfa->state_nums);
    dfa->m_NFA2DFA();
    cout << "-------DFA--------" << endl;
    printAllStates(dfa->DFA_states, dfa->DFA_state_nums, dfa->DFA_e, false);
    return 0;
}