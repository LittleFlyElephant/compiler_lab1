//
// Created by RayChen on 2016/10/25.
//

#include "m_const.h"

bool isInStringArray(const string arr[], string str, const int len){
    for (int i = 0; i < len; ++i) {
        if (arr[i] == str) return true;
    }
    return false;
}

int getType(char c){
    if (c >= 48 && c <= 57) return NUMBER;
    else if ( (c >= 65 && c <= 90) || (c >= 97 && c <= 122) ) return ALPHA;
    else if (c == '\n' || c == 32 || c == '\t') return BLANK;
    else return SYMBOL;
}

bool isDoubleSymbol(string str){
    const int len = 8;
    string doubles[len]= {UNEQU, SELFMINUS, SELFPLUS, JUDEQU, PLUSEQU, MINUSEQU, GREEQU, LESEQU};
    if (isInStringArray(doubles, str, len)) return true;
    return false;
}

bool isReserveWords(string str){
    const int len = 9;
    string ress[len] = {"int", "for", "else", "if", "double", "char", "return", "void", "while"};
    if (isInStringArray(ress, str, len)) return true;
    return false;
}

string getTypeName(int type){
    switch (type){
        case SIGN:
            return "SYMBOL";
        case NUM:
            return "INTEGER";
        case DOUBLE:
            return "DOUBLE";
        case STR:
            return "ID";
        case RES:
            return "RESERVE_WORD";
        case COM_MULTI:
            return "COMMENTS_MULTI";
        case COM_SINGLE:
            return "COMMENTS_SINGLE";
        default:
            return "ERROR";
    }
}