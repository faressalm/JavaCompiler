#include <iostream>
#include <bits/stdc++.h>
using namespace std;
#include "NFA/NFA.h"
#include "LexicalRulesParser/lexical_rules_generator.h"
#include "lexical_analyzer.h"

int main() {
    // rule parsing
    pair<vector<pair<string,int>>, vector<queue<pair<string,bool>>>> REs  =
            LexicalRulesGenerator("..\\lexical_rules.txt").generateNFAs();
    // get NFA
    NFA startNFA =  NFA_builder().build(REs.second,REs.first);
    //get DFS
    //TODO::
    //get MIN DFS
    //TODO::
    //parse program file
    LexicalAnalyzer lexicalAnalyzer = LexicalAnalyzer("..\\testprogram.txt");
    pair<string,string> nameAndValue;
    ofstream outputFile("..\\lexicalOutput.txt", std::ofstream::out);
    while(!(nameAndValue = lexicalAnalyzer.getNextToken()).first.empty())
        outputFile << nameAndValue.first << endl;
    outputFile.close();
    return 0;
}

////LexicalRulesGenerator lexicalAnalyzerGenerator =  LexicalRulesGenerator("..\\lexical_rules.txt");
////lexicalAnalyzerGenerator.generateNFAs();
//vector<queue<pair<string,bool>>> REs ;
//vector<pair<string, int>> tokens;
//queue<pair<string,bool>> re ;
//queue<pair<string,bool>> re2 ;
//re.push({"i",false});
//re.push({"n",false});
//re.push({".",true});
//re.push({"t",false});
//re.push({".",true});
////tokens.push_back({"int", 0});
////REs.push_back(re);
//
//re2.push({"a",false});
////re2.push({"z",false});
//re2.push({"*",true});
///**
//re2.push({"A",false});
//re2.push({"Z",false});
//re2.push({"-",true});
//re2.push({"|",true});
//
//re2.push({"a",false});
//re2.push({"z",false});
//re2.push({"-",true});
//re2.push({"A",false});
//re2.push({"Z",false});
//re2.push({"-",true});
//re2.push({"|",true});
//re2.push({"0",false});
//re2.push({"9",false});
//re2.push({"-",true});
//re2.push({"|",true});
//re2.push({"*",true});
//re2.push({".",true});
//**/
//tokens.push_back({"id", 1});
//REs.push_back(re2);
//NFA_builder builder;
//NFA c = builder.build(REs, tokens);


/**test single RE*/
//    vector<string> punctuations;
//    string ex = "\\=\\= | !\\= | > | >\\= | < | <\\=";
//    queue<pair<string,bool>> q = lexicalAnalyzerGenerator.getPostFix(ex);
//
//    while (!q.empty()){
//
//        cout<<q.front().first<<" ";
//        q.pop();
//    }

/** test all postfixs*/
//for (int i = 0; i < REs.first.size() ; ++i) {
//cout<<REs.first[i].second<<": "<<REs.first[i].first<< ":  ";
//while (!REs.second[i].empty()){
//cout<<REs.second[i].front().first<<" ";
//REs.second[i].pop();
//}
//cout<<endl;
//}