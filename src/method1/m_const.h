//
// Created by RayChen on 2016/10/24.
//

#ifndef TEST2_M_CONST_H
#define TEST2_M_CONST_H
#include <iostream>

using namespace std;

//single symbols
const char SEMI = ';';
const char COMMA = ',';
const char LRBRACE = '(';
const char RRBRACE = ')';
const char LSBRACE = '[';
const char RSBRACE = ']';
const char LBRACE = '{';
const char RBRACE = '}';
const char PLUS = '+';
const char MINUS = '-';
const char NEG = '!';
const char EQU = '=';

//double symbols
const string UNEQU = "!=";
const string SELFPLUS = "++";
const string SELFMINUS = "--";
const string PLUSEQU = "+=";
const string MINUSEQU = "-=";
const string JUDEQU = "==";
const string GREEQU = ">=";
const string LESEQU = "<=";
bool isDoubleSymbol(string str);
bool isReserveWords(string str);

//input types
const int SYMBOL = 1;
const int NUMBER = 2;
const int ALPHA = 3;
const int BLANK = 4;
int getType(char c);

//state types
const int SIGN = 1;
const int NUM = 2;
const int ZERO = 12;
const int DOUBLE_START = 7;
const int DOUBLE = 11;
const int STR = 3;
const int RES = 4;
const int NUL = 5;
const int COM_SINGLE = 8;
const int COM_MULTI = 9;
const int COM_MULTI_OUT = 10;
const int COM_END = 6;
string getTypeName(int type);

#endif //TEST2_M_CONST_H
