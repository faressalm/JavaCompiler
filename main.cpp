#include <iostream>
#include <bits/stdc++.h>
#include "LexicalAnalyzer/NFA/NFA.h"
#include "LexicalAnalyzer/LexicalRulesParser/lexical_rules_generator.h"
#include "LexicalAnalyzer/lexical_analyzer.h"
#include "LexicalAnalyzer/DFA/DFA.h"
#include "Parser/ProductionGenerator/production_generator.h"
#include "Parser/ParsingTable/parser_utils.h"
#include "Parser/TopDownParser.h"
using namespace std;
void printParsingTable(ProductionGenerator &productionGenerator, vector<ProductionRule> &productionRules, ParserUtils &parserUtils){
    unordered_set<string> terminals = productionGenerator.getSetOfTerminals(productionRules);
    parserUtils.print_parsing_table("../Parsing_Table.csv", terminals);
    ParserToken nonterm = ParserToken(ParserToken::Type::NonTerminal, "EXPRESSION");
    ParserToken term = ParserToken(ParserToken::Type::Terminal, "**");
    auto a = parserUtils.get_entry(nonterm, term);
}
int main() {
    // Parsing Grammar Rules
    ProductionGenerator productionGenerator = ProductionGenerator("../CFG.txt");
    vector<ProductionRule> productionRules = productionGenerator.getProductions();
    // LL(1)
    productionRules = productionGenerator.eliminateLR(productionRules, productionRules[0]);
    productionRules = productionGenerator.leftFactor(productionRules, productionRules[0]);
    // Create Parsing table
    ParserUtils parserUtils = ParserUtils(productionRules);
    ParserToken start =ParserToken(ParserToken::NonTerminal, productionRules[0].name);
    ParserToken match = ParserToken(ParserToken::Terminal,"int");
    cout<< parserUtils.get_entry(start, match).second[0].name;
    TopDownParser topDownParser = TopDownParser(parserUtils,
                                                ParserToken(ParserToken::NonTerminal, productionRules[0].name));
    topDownParser.parse();

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