//
// Created by Fares Salm on 12/5/2022.
//

#ifndef JAVACOMPILER_LEXICAL_ANALYZER_H
#define JAVACOMPILER_LEXICAL_ANALYZER_H
#include <bits/stdc++.h>
using namespace std;
class LexicalAnalyzer{
private:
    const unordered_set<char> skipCharacters = {' ','\t','\n'};
    ifstream inputFile;
    int inputLineIndex = 0;
    string inputLine = "";
    char getNextChar();

public:
    LexicalAnalyzer(string inputProgramPath);
    // token name, token value
    pair<string,string> getNextToken();
};
#endif //JAVACOMPILER_LEXICAL_ANALYZER_H
