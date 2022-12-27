#include <iostream>
#include <bits/stdc++.h>
#include "LexicalAnalyzer/NFA/NFA.h"
#include "LexicalAnalyzer/LexicalRulesParser/lexical_rules_generator.h"
#include "LexicalAnalyzer/lexical_analyzer.h"
#include "LexicalAnalyzer/DFA/DFA.h"
#include "Parser/ProductionGenerator/production_generator.h"
#include "Parser/ParsingTable/parser_utils.h"

using namespace std;
void lexical(){
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
}
int main() {
    ProductionGenerator productionGenerator = ProductionGenerator("../CFG.txt");
    vector<ProductionRule> productionRules = productionGenerator.getProductions();
    unordered_set<string> terminals = productionGenerator.getSetOfTerminals(productionRules);
    productionRules = productionGenerator.eliminateLR(productionRules, productionRules[0]);
    productionRules = productionGenerator.leftFactor(productionRules, productionRules[0]);
    //ParserUtils parserUtils = ParserUtils(productionRules);
    for(auto &productionRule: productionRules){
        cout<< productionRule.name<<" : \n";
        for(auto & rule: productionRule.rules){
            for(auto &token : rule)
                cout<< ParserToken::typeName(token.type) << "," << token.name<<" # ";
            cout<<endl;
        }
        cout<<"----------------------------"<<endl;
    }
    return 0;

}

/**
    for(auto &productionRule: productionRules){
        cout<< productionRule.name<<" : \n";
        for(auto & rule: productionRule.rules){
            for(auto &token : rule)
                cout<< ParserToken::typeName(token.type) << "," << token.name<<" # ";
            cout<<endl;
        }
        cout<<"----------------------------"<<endl;
    }

 **/