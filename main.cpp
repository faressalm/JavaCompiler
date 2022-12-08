#include <iostream>
#include <bits/stdc++.h>
using namespace std;
#include "NFA/NFA.h"
#include "LexicalRulesParser/lexical_rules_generator.h"
#include "lexical_analyzer.h"
#include "DFA/DFA.h"

int main() {
    // rule parsing
    pair<vector<pair<string,int>>, vector<queue<pair<string,bool>>>> REs  =
            LexicalRulesGenerator("..\\lexical_rules.txt").generateNFAs();
    // get NFA
    NFA startNFA =  NFA_builder().build(REs.second,REs.first);
    //get DFA
    DFA dfa = DFA_builder::build_dfa(startNFA);;
    dfa.transition_table( "..\\Transition_Table.csv",startNFA.chars);
    //parse program file
    cout << "Number of states after minimization: "<<dfa.states.size() <<endl<<"=========================================================="<<endl;
    LexicalAnalyzer lexicalAnalyzer = LexicalAnalyzer("..\\testprogram.txt",dfa,dfa.states[0]);
    pair<string,string> nameAndValue;
    ofstream outputFile("..\\lexicalOutput.txt", std::ofstream::out);
    while(!lexicalAnalyzer.fileClosed){
        if(!(nameAndValue = lexicalAnalyzer.getNextToken()).first.empty())
            outputFile << nameAndValue.first<<endl;
    }
    outputFile.close();

    return 0;
}