#include <iostream>

#include "reader.h"
#include "m_const.h"

using namespace std;

const int max_tokens = 1000;
struct Token{
    int type;
    string name;
}token[max_tokens];
int tokens = 0;
int m_state = NUL;
bool donable = false;

void addToken(int type, string name){
    if (name == "")
        return;
    tokens ++;
    if (type == STR && isReserveWords(name)){
        token[tokens-1].type = RES;
    } else
        token[tokens-1].type = type;
    token[tokens-1].name = name;
}

void printToken(){
    for (int i = 0; i < tokens; ++i) {
        cout << "<" <<getTypeName(token[i].type) << ", " << token[i].name << ">" << endl;
    }
}

void analyze(string s){
    int p = 0;
    string current_token = "";
    m_state = NUL;
    bool in_com = false; //判断是否在注释中
    while (p <= s.length()) {
        int type = getType(s[p]);
        if (type == BLANK && m_state != NUL && !in_com){ //空白符
            addToken(m_state, current_token);
            current_token = "";
            m_state = NUL;
        } else{
            switch (m_state) {
                case SIGN:{
                    if (type == SYMBOL) {
                        if (current_token.length() == 1){
                            if (current_token == "/" && s[p] == '*'){
                                in_com = true;
                                m_state = COM_MULTI;
                            } else if (current_token == "/" && s[p] == '/'){
                                in_com = true;
                                m_state = COM_SINGLE;
                            } else if (!(isDoubleSymbol(current_token+s[p]))){
                                addToken(SIGN, current_token);
                                current_token = "";
                            }
                        } else{
                            addToken(SIGN, current_token);
                            current_token = "";
                        }
                        donable = true;
                    }
                    else {
                        addToken(SIGN, current_token);
                        current_token = "";
                        if (type == NUMBER) {
                            if (s[p] == '0') m_state = ZERO;
                            else m_state = NUM;
                        }
                        else if (type == ALPHA) m_state = STR;
                        donable = true;
                    }
                    break;
                }
                case ZERO:{
                    if (s[p] == '.') {
                        m_state = DOUBLE_START;
                    } else {
                        addToken(NUM, current_token);
                        current_token = "";
                        if (type == SYMBOL) m_state = SIGN;
                        else if (type == ALPHA) m_state = STR;
                        else if (type == NUMBER) {
                            if (s[p] == '0') m_state = ZERO;
                            else m_state = NUM;
                        }
                    }
                    break;
                }
                case NUM:{
                    if (s[p] == '.'){
                        m_state = DOUBLE_START;
                    } else if (type != NUM){
                        addToken(NUM, current_token);
                        current_token = "";
                        if (type == SYMBOL) m_state = SIGN;
                        else if (type == ALPHA) m_state = STR;
                    }
                    break;
                }
                case DOUBLE_START:{
                    if (type != NUMBER) {
                        //error
                    } else {
                        m_state = DOUBLE;
                    }
                }
                case DOUBLE:{
                    if (type != NUMBER){
                        addToken(DOUBLE, current_token);
                        current_token = "";
                        if (type == SYMBOL) m_state = SIGN;
                        else if (type == ALPHA) m_state = STR;
                    }
                }
                case STR:{
                    if (type == SYMBOL){
                        addToken(STR, current_token);
                        current_token = "";
                        m_state = SIGN;
                    }
                    break;
                }
                case NUL:{
                    switch (type){
                        case SYMBOL: m_state = SIGN;
                            break;
                        case NUMBER:
                            if (s[p] == '0') m_state = ZERO;
                            else m_state = NUM;
                            break;
                        case ALPHA: m_state = STR;
                            break;
                    }
                    break;
                }
                case COM_SINGLE:{
                    if (s[p] == '\n') {
                        addToken(COM_SINGLE, current_token);
                        current_token = "";
                        m_state = NUL;
                        in_com = false;
                    }
                    break;
                }
                case COM_MULTI:{
                    if (s[p] == '*') {
                        m_state = COM_MULTI_OUT;
                    }
                    break;
                }
                case COM_MULTI_OUT:{
                    if (s[p] == '/') {
                        m_state = COM_END;
                    } else if (s[p] != '*') m_state = COM_MULTI;
                    break;
                }
                case COM_END: { //多行注释结束
                    addToken(COM_MULTI, current_token);
                    current_token = "";
                    in_com = false;
                    switch (type){
                        case SYMBOL: m_state = SIGN;
                            break;
                        case NUMBER: m_state = NUM;
                            break;
                        case ALPHA: m_state = STR;
                            break;
                    }
                    break;
                }
                default: break;
            }
        }
        if (type!=BLANK || in_com) current_token += s[p];
        p++;
    }
}

int main() {
    string s = readFile("test.txt");
    analyze(s);
    printToken();
    return 0;
}