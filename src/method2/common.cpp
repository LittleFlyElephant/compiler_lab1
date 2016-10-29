//
// Created by RayChen on 2016/10/29.
//

#include "common.h"
#include <iostream>
#include <queue>
#include <set>

using namespace std;

bool isChar(char c){
    return ((c>=97) && (c<=122));
}

void newAstate(Sta state[], int& state_nums){
    state_nums ++;
    state[state_nums-1].is_start = false;
    state[state_nums-1].is_end = false;
    state[state_nums-1].first_edge = -1;
}

void newAedge(int start, int end, char path, Sta state[], Edge e[], int& e_nums){
    e_nums ++;
    e[e_nums-1].des = end;
    e[e_nums-1].path = path;
    e[e_nums-1].next_edge = state[start].first_edge;
    state[start].first_edge = e_nums-1;
}

void printAllStates(Sta state[], int state_nums, Edge e[], bool b){//打印所有状态
    queue<int> q;
    for (int i = 0; i < state_nums; ++i) {
        if(state[i].is_start) {
            q.push(i);
            break;
        }
    }
    bool vis[state_nums];
    for (int j = 0; j < state_nums; ++j) {
        vis[j] = false;
    }
    vis[q.front()] = true;
    while (!q.empty()){
        int sta = q.front();
        q.pop();
        if (state[sta].is_start) cout << "[start]";
        cout << "state: " << sta;
        if (state[sta].is_end) cout << "[end ]";
        cout << endl;
        if (b) {
            for (set<int>::iterator i = state[sta].rep.begin(); i != state[sta].rep.end(); ++i) {
                cout << *i << " ";
            }
            cout << endl;
        }
        int e_i = state[sta].first_edge;
        while (e_i != -1){
//            cout << "  edge: ";
//            cout << "from " << sta << " to " << e[e_i].des << " by path: " << e[e_i].path << endl;
            cout << "  " << sta << "-->" << e[e_i].des << "[" << e[e_i].path << "]" << endl;
            if (!vis[e[e_i].des]) {
                q.push(e[e_i].des);
                vis[e[e_i].des] = true;
            }
            e_i = e[e_i].next_edge;
        }
    }
}
