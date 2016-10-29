//
// Created by RayChen on 2016/10/29.
//

#include "NFA2DFA.h"
#include <stack>
#include <queue>
#include <vector>

bool NFA2DFA::EPclosure(set<int>& s){//求e-closure
    stack<int> st;
    set<int>::iterator sit;
    bool can_end = false;
    bool vis[state_nums];
    for (int j = 0; j < state_nums; ++j) {
        vis[j] = false;
    }
    for (sit = s.begin(); sit != s.end(); sit++){
        st.push(*sit);
        vis[*sit] = true;
    }
    while (!st.empty()){
        int t = st.top();
        st.pop();
        int e_i = NFA_states[t].first_edge;
        while (e_i != -1){
            if (NFA_e[e_i].path == EP) {
                if (!vis[NFA_e[e_i].des]){
                    if (NFA_states[NFA_e[e_i].des].is_end) can_end = true;
                    s.insert(NFA_e[e_i].des);
                    st.push(NFA_e[e_i].des);
                    vis[NFA_e[e_i].des] = true;
                }
            }
            e_i = NFA_e[e_i].next_edge;
        }
    }
    return can_end;
}

void NFA2DFA::m_NFA2DFA(){//NFA转DFA
    queue<set<int>> q;
    queue<int> q_state_num; //记录访问过的集合的state number
    vector<set<int>> vis;
    vector<int> vis_state_num; //记录访问过的集合的state number
    set<int> first;
    set<char>::iterator sit;
    for (int i = 0; i < state_nums; ++i) {
        if(NFA_states[i].is_start) {
            NFA_states[i].is_start = false;
            first.insert(i);
            break;
        }
    }
    bool b = EPclosure(first);
    newAstate(DFA_states, DFA_state_nums);
    DFA_states[DFA_state_nums-1].is_start = true;
    DFA_states[DFA_state_nums-1].is_end = b;
    DFA_states[DFA_state_nums-1].rep = first;
    q.push(first);
    q_state_num.push(DFA_state_nums-1);
    vis.insert(vis.end(), first);
    vis_state_num.insert(vis_state_num.end(), DFA_state_nums-1);
    while (!q.empty()){
        set<int> stas = q.front(); //stas = T
        q.pop();
        int in = q_state_num.front();
        q_state_num.pop();
        for (sit = path_set.begin(); sit != path_set.end(); sit++){ // for(输入的每个符号)
            char path = *sit;
            set<int> s;
            set<int>::iterator sit2;
            bool can_end = false;
            for (sit2 = stas.begin(); sit2 != stas.end(); sit2++){ // move(T,a)
                int sta = *sit2;
                int e_i = NFA_states[sta].first_edge;
                while (e_i != -1){
                    if (NFA_e[e_i].path == path) {
                        if (NFA_states[NFA_e[e_i].des].is_end) can_end = true;
                        s.insert(NFA_e[e_i].des);
                    }
                    e_i = NFA_e[e_i].next_edge;
                }
            }
            bool b = EPclosure(s); //e-closure(move(T,a))
            int sta_num = -1;
            for (int i = 0; i < vis.size(); ++i) {
                if (s == vis.at(i)){
                    sta_num = vis_state_num.at(i);
                    break;
                }
            }
            if (sta_num < 0) { //不在q中
                newAstate(DFA_states, DFA_state_nums);
                DFA_states[DFA_state_nums-1].is_end = b || can_end;
                DFA_states[DFA_state_nums-1].rep = s;
                sta_num = DFA_state_nums-1;
                q.push(s);
                q_state_num.push(DFA_state_nums-1);
                vis.insert(vis.end(), s);
                vis_state_num.insert(vis_state_num.end(), DFA_state_nums-1);
            }
            if (!s.empty()){
                newAedge(in, sta_num, path, DFA_states, DFA_e, e_nums);
            }
        }
    }
}
