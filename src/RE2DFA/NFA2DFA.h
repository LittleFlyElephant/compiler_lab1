//
// Created by RayChen on 2016/10/29.
//

#ifndef RE2DFA_NFA2DFA_H
#define RE2DFA_NFA2DFA_H

#include "common.h"
#include <set>
#include <iostream>
using namespace std;

class NFA2DFA {
public:
    Sta DFA_states[max_states];
    Edge DFA_e[max_edges];
    int DFA_state_nums;
    int e_nums;
    void m_NFA2DFA();
    NFA2DFA(Sta nfa[], Edge e[], set<char> path, int num){
        for (int i = 0; i < max_states; ++i) {
            this->NFA_states[i] = nfa[i];
        }
        for (int i = 0; i < max_edges; ++i) {
            this->NFA_e[i] = e[i];
        }
        this->path_set = path;
        this->state_nums = num;
    }
private:
    Sta NFA_states[max_states];
    Edge NFA_e[max_edges];
    int state_nums;
    set<char> path_set;
    bool EPclosure(set<int>& s);
};


#endif //RE2DFA_NFA2DFA_H
