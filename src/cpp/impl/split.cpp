#include "../dec/split.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstring>

vector<string> split(char* a, char pattern) {
    int C_STRING_LENGTH = strlen(a);
    string str(C_STRING_LENGTH, 1);
    memmove(&str[0], a, C_STRING_LENGTH);
    int posInit = 0;
    int posFound = 0;
    string splitted;
    vector<string> results;
    
    while(posFound >= 0){
        posFound = str.find(pattern, posInit);
        splitted = str.substr(posInit, posFound - posInit);
        posInit = posFound + 1;
        results.push_back(splitted);
    }
    
    return results;
}