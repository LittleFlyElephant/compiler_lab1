//
// Created by RayChen on 2016/10/29.
//

#ifndef RE2DFA_COMMON_H
#define RE2DFA_COMMON_H

#include <set>
const int max_states = 1000;
const int max_edges = 1000;
const char EP = '&';

struct Edge{
    int des;
    char path;
    int next_edge;
};
struct Sta{
    int first_edge;
    bool is_start;
    bool is_end;
    std::set<int> rep;
};

bool isChar(char c);//判断是否为a-z
void newAstate(Sta state[], int& state_nums);//新建一个状态
void newAedge(int start, int end, char path, Sta state[], Edge e[], int& e_nums);//新建一条边
void printAllStates(Sta state[], int state_nums, Edge e[], bool b);//打印所有状态

#endif //RE2DFA_COMMON_H
