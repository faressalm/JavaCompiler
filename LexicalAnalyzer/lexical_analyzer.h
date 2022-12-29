//
// Created by Fares Salm on 12/5/2022.
//

#ifndef JAVACOMPILER_LEXICAL_ANALYZER_H
#define JAVACOMPILER_LEXICAL_ANALYZER_H
#include <bits/stdc++.h>
#include "DFA/DFA.h"
using namespace std;
class LexicalAnalyzer{
private:
    unordered_set<char> splitCharacters = {' ', '\t', '\n'};
    DFA_State startState;
    DFA dfa;
    ifstream inputFile;
    int inputLineIndex = 0;
    int currentInputLine = 0;
    string inputLine = "";
    char prevChar = '\n';
    char getNextChar();
    bool isSplitChar(char c);
    bool checkForPrevChar(string & rawToken,stack<DFA_State> & maxMunch,DFA_State &dfaState);

public:
    bool fileClosed = false;
    explicit LexicalAnalyzer(string inputProgramPath,DFA dfa,DFA_State startState);
    // token name, token value
    pair<string,string> getNextToken();


};
#endif //JAVACOMPILER_LEXICAL_ANALYZER_H