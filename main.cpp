#include <iostream>
#include <bits/stdc++.h>
using namespace std;
#include "NFA/NFA.h"
#include "LexicalRulesParser/lexical_rules_generator.h"
#include "lexical_analyzer.h"
#include "DFA/DFA.h"

int main() {
    DFA_State a = DFA_State(0);
    DFA_State b = DFA_State(1);
    DFA_State c = DFA_State(2);
    DFA_State d = DFA_State(3);
    DFA_State e = DFA_State(4);
    DFA_State f = DFA_State(5);

    a.accepting = false;
    b.accepting = false;
    c.accepting = true;
    c.acceptance_state = 'a';
    d.accepting = true;
    d.acceptance_state = 'a';
    e.accepting = true;
    e.acceptance_state = 'a';

    unordered_map<string, int> ma;
    ma.insert({"0", 1});
    ma.insert({"1", 2});

    unordered_map<string, int> mb;
    mb.insert({"0", 0});
    mb.insert({"1", 3});

    unordered_map<string, int> mc;
    mc.insert({"0", 4});
    mc.insert({"1", 5});

    unordered_map<string, int> md;
    md.insert({"0", 4});
    md.insert({"1", 5});

    unordered_map<string, int> me;
    me.insert({"0", 4});
    me.insert({"1", 5});

    unordered_map<string, int> mf;
    mf.insert({"0", 5});
    mf.insert({"1", 5});

    a.transitions = ma;
    b.transitions = mb;
    c.transitions = mc;
    d.transitions = md;
    e.transitions = me;
    f.transitions = mf;

    vector<DFA_State> v;
    v.push_back(a);
    v.push_back(b);
    v.push_back(c);
    v.push_back(d);
    v.push_back(e);
    v.push_back(f);
    DFA test = DFA_builder::minimize_dfa(v);

    // rule parsing
    pair<vector<pair<string,int>>, vector<queue<pair<string,bool>>>> REs  =
            LexicalRulesGenerator("..\\lexical_rules.txt").generateNFAs();
    // get NFA
    NFA startNFA =  NFA_builder().build(REs.second,REs.first);
    //get DFA
    DFA dfa = DFA_builder::build_dfa(startNFA);
    cout << (int)dfa.states.size() << "\n";
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