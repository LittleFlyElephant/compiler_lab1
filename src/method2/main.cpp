#include <iostream>
#include <stack>
#include <queue>
#include <set>
#include "main.h"

using namespace std;

const int max_states = 1000;
const int max_edges = 1000;
const char EP = '&';

struct edge{
    int des;
    char path;
    int next_edge;
}e[max_edges];
int e_nums = 0;

struct sta{
    int first_edge;
    bool is_start;
    bool is_end;
    set<int> rep;
}state[max_states];
int state_nums = 0;

stack<string> stack_char;
stack<char> stack_op;
set<char> path_set;

string combine(){ //从操作符栈中取一个操作符,操作数栈中取2个操作数组合
    char t = stack_op.top();
    stack_op.pop();
    string op2 = stack_char.top();
    stack_char.pop();
    string op1 = stack_char.top();
    stack_char.pop();
    return op1+op2+t;
}

bool isChar(char c){//判断是否为a-z
    return ((c>=97) && (c<=122));
}

void newAstate(){//新建一个状态
    state_nums ++;
    state[state_nums-1].is_start = false;
    state[state_nums-1].is_end = false;
    state[state_nums-1].first_edge = -1;
}

void newAedge(int start, int end, char path){//新建一条边
    e_nums ++;
    e[e_nums-1].des = end;
    e[e_nums-1].path = path;
    e[e_nums-1].next_edge = state[start].first_edge;
    state[start].first_edge = e_nums-1;
}

void newFromChar(char c){//单个字母转换为NFA
    newAstate();
    state[state_nums-1].is_start = true;
    newAstate();
    state[state_nums-1].is_end = true;
    newAedge(state_nums-2, state_nums-1, c);
}

void joinTwoState(int& in1, int& out1, int& in2, int& out2){//NFA连接
    state[out1].is_end = false;
    state[in2].is_start = false;
    state[out1].first_edge = state[in2].first_edge;
    state[in2].first_edge = -1;
    out1 = out2;
    in2 = -1;
    out2 = -1;
}

void orTwoState(int& in1, int& out1, int& in2, int& out2){//NFA或运算
    state[in1].is_start = false;
    state[in2].is_start = false;
    state[out1].is_end = false;
    state[out2].is_end = false;
    newAstate();
    newAedge(state_nums-1, in1, EP);
    newAedge(state_nums-1, in2, EP);
    state[state_nums-1].is_start = true;
    newAstate();
    newAedge(out1, state_nums-1, EP);
    newAedge(out2, state_nums-1, EP);
    state[state_nums-1].is_end = true;
    in1 = state_nums-2;
    out1 = state_nums-1;
    in2 = -1;
    out2 = -1;
}

void loopOneState(int& in, int& out){//NFA*运算
    state[in].is_start = false;
    state[out].is_end = false;
    newAedge(out, in, EP);
    newAstate();
    state[state_nums-1].is_start = true;
    newAstate();
    state[state_nums-1].is_end = true;
    newAedge(state_nums-2, in, EP);
    newAedge(state_nums-2, state_nums-1, EP);
    newAedge(out, state_nums-1, EP);
    in = state_nums-2;
    out = state_nums-1;
}

void addDotToRE(string& re){//增加连接符到RE
    if (isChar(re[0])) path_set.insert(re[0]);
    for (int i = 1; i < re.length(); ++i) {
        if ((isChar(re[i-1]) || re[i-1] == '*' || re[i-1] == ')') && isChar(re[i]))
            re.insert(i,1,'.');
        if (isChar(re[i])) path_set.insert(re[i]);
    }
}

void RE2POST(string& re){//中缀转后缀
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

void RE2NFA(string re){//正则表达式转NFA
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

bool EPclosure(set<int>& s){//求e-closure
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
        int e_i = state[t].first_edge;
        while (e_i != -1){
            if (e[e_i].path == EP) {
                if (!vis[e[e_i].des]){
                    if (state[e[e_i].des].is_end) can_end = true;
                    s.insert(e[e_i].des);
                    st.push(e[e_i].des);
                    vis[e[e_i].des] = true;
                }
            }
            e_i = e[e_i].next_edge;
        }
    }
    return can_end;
}

void NFA2DFA(){//NFA转DFA
    queue<set<int>> q;
    queue<int> q_state_num; //记录访问过的集合的state number
    vector<set<int>> vis;
    vector<int> vis_state_num; //记录访问过的集合的state number
    set<int> first;
    set<char>::iterator sit;
    for (int i = 0; i < state_nums; ++i) {
        if(state[i].is_start) {
            state[i].is_start = false;
            first.insert(i);
            break;
        }
    }
    bool b = EPclosure(first);
    newAstate();
    state[state_nums-1].is_start = true;
    state[state_nums-1].is_end = b;
    state[state_nums-1].rep = first;
    q.push(first);
    q_state_num.push(state_nums-1);
    vis.insert(vis.end(), first);
    vis_state_num.insert(vis_state_num.end(), state_nums-1);
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
                int e_i = state[sta].first_edge;
                while (e_i != -1){
                    if (e[e_i].path == path) s.insert(e[e_i].des);
                    if (state[e[e_i].des].is_end) can_end = true;
                    e_i = e[e_i].next_edge;
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
                newAstate();
                state[state_nums-1].is_end = b || can_end;
                state[state_nums-1].rep = s;
                sta_num = state_nums-1;
                q.push(s);
                q_state_num.push(state_nums-1);
                vis.insert(vis.end(), s);
                vis_state_num.insert(vis_state_num.end(), state_nums-1);
            }
            if (!s.empty()){
                newAedge(in, sta_num, path);
            }
        }
    }
}

void printAllStates(){//打印所有状态
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

int main() {
    string re;
    cin >> re;
    addDotToRE(re);
    cout << re << endl;
    RE2POST(re);
    cout << re << endl;
    RE2NFA(re);
    cout << "-------NFA--------" << endl;
    printAllStates();
    NFA2DFA();
    cout << "-------DFA--------" << endl;
    printAllStates();
    return 0;
}