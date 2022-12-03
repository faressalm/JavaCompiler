//
// Created by Fares Salm on 12/3/2022.
//
#ifndef JAVACOMPILER_LEXICAL_RULES_GENERATOR_H
#define JAVACOMPILER_LEXICAL_RULES_GENERATOR_H
#include <bits/stdc++.h>
using namespace std;
class LexicalAnalyzerGenerator{
private:
    const string lexicalRulesPath;
    // "." stands for concatenation
    unordered_map<string,int>  fixedRulesChars = { {".",5},{"-",10}, {"|",6}, {"+",8}, {"(",0}, {")",0}, {"*",8}};
public:
    LexicalAnalyzerGenerator(string lexicalRulesPath);
    pair<vector<pair<string,int>>, vector<queue<pair<string,bool>>>> generateNFAs();
//private:
    //assume each keyword is separated from others with spaces
    void addKeywords(vector<string> &keywords,string & lexicalRuleLine);

    void addPunctuations(vector<string> &punctuations,string & lexicalRuleLine);

    /** first -> token name, second-> index of assign operator for RE or RD*/
    pair<string,int> getTokenName(string lexicalRuleLine);

    /** replace RDs in RE and RD with it only Terminals*/
    // assume no Expression will be defined  with RD before this RD is defined
    // assume RDs name will if one's name is part of other one the smaller will be defined first
    void replaceRDs(string &expression,vector<pair<string,string>> &RDs);

    queue<pair<string,bool>> getPostFix(string expression);

    bool isOperator (string nextChar);

};
#endif //JAVACOMPILER_LEXICAL_RULES_GENERATOR_H
