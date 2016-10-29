//
// Created by RayChen on 2016/10/29.
//

#ifndef C_LEXICAL_ANALYZER_ANALYZE_H
#define C_LEXICAL_ANALYZER_ANALYZE_H

#include <iostream>
using namespace std;

void addToken(int type, string name);
void analyze(string s);
void printToken();

#endif //C_LEXICAL_ANALYZER_ANALYZE_H
