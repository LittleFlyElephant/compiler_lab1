//
// Created by RayChen on 2016/10/29.
//

#include "RE2NFA.h"

string RE2NFA::combine() { //从操作符栈中取一个操作符,操作数栈中取2个操作数组合
    char t = stack_op.top();
    stack_op.pop();
    string op2 = stack_char.top();
    stack_char.pop();
    string op1 = stack_char.top();
    stack_char.pop();
    return op1+op2+t;
}

void RE2NFA::newFromChar(char c){//单个字母转换为NFA
    newAstate(NFA_states, state_nums);
    NFA_states[state_nums-1].is_start = true;
    newAstate(NFA_states, state_nums);
    NFA_states[state_nums-1].is_end = true;
    newAedge(state_nums-2, state_nums-1, c, NFA_states, NFA_e, e_nums);
}

void RE2NFA::joinTwoState(int& in1, int& out1, int& in2, int& out2){//NFA连接
    NFA_states[out1].is_end = false;
    NFA_states[in2].is_start = false;
    NFA_states[out1].first_edge = NFA_states[in2].first_edge;
    NFA_states[in2].first_edge = -1;
    out1 = out2;
    in2 = -1;
    out2 = -1;
}

void RE2NFA::orTwoState(int& in1, int& out1, int& in2, int& out2){//NFA或运算
    NFA_states[in1].is_start = false;
    NFA_states[in2].is_start = false;
    NFA_states[out1].is_end = false;
    NFA_states[out2].is_end = false;
    newAstate(NFA_states, state_nums);
    newAedge(state_nums-1, in1, EP, NFA_states, NFA_e, e_nums);
    newAedge(state_nums-1, in2, EP, NFA_states, NFA_e, e_nums);
    NFA_states[state_nums-1].is_start = true;
    newAstate(NFA_states, state_nums);
    newAedge(out1, state_nums-1, EP, NFA_states, NFA_e, e_nums);
    newAedge(out2, state_nums-1, EP, NFA_states, NFA_e, e_nums);
    NFA_states[state_nums-1].is_end = true;
    in1 = state_nums-2;
    out1 = state_nums-1;
    in2 = -1;
    out2 = -1;
}

void RE2NFA::loopOneState(int& in, int& out){//NFA*运算
    NFA_states[in].is_start = false;
    NFA_states[out].is_end = false;
    newAedge(out, in, EP, NFA_states, NFA_e, e_nums);
    newAstate(NFA_states, state_nums);
    NFA_states[state_nums-1].is_start = true;
    newAstate(NFA_states, state_nums);
    NFA_states[state_nums-1].is_end = true;
    newAedge(state_nums-2, in, EP, NFA_states, NFA_e, e_nums);
    newAedge(state_nums-2, state_nums-1, EP, NFA_states, NFA_e, e_nums);
    newAedge(out, state_nums-1, EP, NFA_states, NFA_e, e_nums);
    in = state_nums-2;
    out = state_nums-1;
}

void RE2NFA::addDotToRE(string& re){//增加连接符到RE
    for (int i = 1; i < re.length(); ++i) {
        if ((isChar(re[i-1]) || re[i-1] == '*' || re[i-1] == ')') && isChar(re[i]))
            re.insert(i,1,'.');
    }
}

void RE2NFA::RE2POST(string& re){//中缀转后缀
    for (int i = 0; i < re.length(); ++i) {
        string s = "";
        s = s + re[i];
        if (isChar(re[i])) stack_char.push(s);
        else if (re[i] == ')'){
            while (stack_op.top() != '(') stack_char.push(combine());
            stack_op.pop();
        }
        else if (re[i] == '*') {
            string op = stack_char.top();
            stack_char.pop();
            stack_char.push(op+'*');
        }
        else if (stack_op.empty() || stack_op.top() == '(') stack_op.push(re[i]);
        else {
            stack_char.push(combine());
            stack_op.push(re[i]);
        }
    }
    if (!stack_op.empty()){
        stack_char.push(combine());
    }
    re = stack_char.top();
}

void RE2NFA::m_RE2NFA(string re){//正则表达式转NFA
    int in1 = -1; // start state 1
    int out1  = -1; // end state 1
    int in2 = -1; // start state 2
    int out2 = -1; // end state 2
    for (int i = 0; i < re.length(); ++i) {
        if (isChar(re[i]) && in1 == -1){
            newFromChar(re[i]);
            in1 = state_nums-2;
            out1 = state_nums-1;
        } else if (isChar(re[i]) && in2 == -1){
            newFromChar(re[i]);
            in2 = state_nums-2;
            out2 = state_nums-1;
        } else if (re[i] == '.'){
            joinTwoState(in1, out1, in2, out2);
        } else if (re[i] == '|'){
            orTwoState(in1, out1, in2, out2);
        } else if (re[i] == '*' && in2 == -1) {
            loopOneState(in1, out1);
        } else if (re[i] == '*') {
            loopOneState(in2, out2);
        }
    }
}