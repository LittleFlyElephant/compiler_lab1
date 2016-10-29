//
// Created by RayChen on 2016/10/29.
//

#ifndef RE2DFA_RE2NFA_H
#define RE2DFA_RE2NFA_H

#include "common.h"
#include <stack>
#include <iostream>
using namespace std;

class RE2NFA {
public:
    Sta NFA_states[max_states];
    Edge NFA_e[max_edges];
    int state_nums;
    int e_nums;
    void addDotToRE(string& re);
    void RE2POST(string& re);
    void m_RE2NFA(string re);
private:
    stack<string> stack_char;
    stack<char> stack_op;
    string combine();
    void newFromChar(char c);
    void joinTwoState(int& in1, int& out1, int& in2, int& out2);
    void orTwoState(int& in1, int& out1, int& in2, int& out2);
    void loopOneState(int& in, int& out);
};

#endif //RE2DFA_RE2NFA_H
