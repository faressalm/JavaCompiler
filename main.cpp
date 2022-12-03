#include <iostream>
#include <bits/stdc++.h>
using namespace std;
#include "NFA/NFA.h"
#include "LexicalRulesParser/lexical_rules_generator.h"

int main() {
    LexicalAnalyzerGenerator lexicalAnalyzerGenerator =  LexicalAnalyzerGenerator("..\\lexical_rules.txt");
    pair<vector<pair<string,int>>, vector<queue<pair<string,bool>>>> bb = lexicalAnalyzerGenerator.generateNFAs();
    vector<pair<string,int>> tokensAndPriorities  = bb.first;
    vector<queue<pair<string,bool>>> postfixes = bb.second;
    vector<string> punctuations;
    string ex = "\\=\\= | !\\= | > | >\\= | < | <\\=";
//    queue<pair<string,bool>> q = lexicalAnalyzerGenerator.getPostFix(ex);
//
//    while (!q.empty()){
//
//        cout<<q.front().first<<" "<<endl;
//        q.pop();
//    }

    for(auto &b: postfixes){
//        cout<< b.first<< ":  ";
//        while (!b.second.empty()){
//            cout<<b.second.front().first<<" ";
//            b.second.pop();
//        }
//        cout<<endl;
    }
    return 0;
}

////LexicalAnalyzerGenerator lexicalAnalyzerGenerator =  LexicalAnalyzerGenerator("..\\lexical_rules.txt");
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
