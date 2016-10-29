#include <iostream>

#include "reader.h"
#include "analyze.h"

using namespace std;

int main() {
    string s = readFile("test2.txt");
    analyze(s);
    printToken();
    return 0;
}