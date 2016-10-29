//
// Created by RayChen on 2016/10/24.
//


#include <fstream>
#include <iostream>
#include "reader.h"

using namespace std;

const int maxLen = 100;

string readFile(string fileName){
    fstream f;
    string ret = "";
    f.open(fileName);
    if (f.good()){
        char in[maxLen];
        while (!f.eof()) {
            f.getline(in, maxLen);
            for (int i = 0; in[i] != '\0'; ++i) {
                ret = ret + in[i];
            }
            ret = ret + '\n';
        }
    } else {
        cout << "no file!";
    }
    f.close();
    return ret;
}