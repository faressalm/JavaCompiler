//
// Created by Fares Salm on 12/22/2022.
//

#ifndef JAVACOMPILER_PRODUCTION_GENERATOR_H
#define JAVACOMPILER_PRODUCTION_GENERATOR_H
#include <bits/stdc++.h>
#include "../production_rule.h"

using namespace std;
class ProductionGenerator {
private:
    const string productionsPath;
public:
    ProductionGenerator(string productionsPath);
    vector<ProductionRule> getProductions();
    unordered_set<string> getSetOfTerminals(vector<ProductionRule> productionRules);
private:
    const unordered_set<char> skipCharacters = {' ', '\t', '\n'};
    vector<string> getRawProductions(string buffer);
    vector<ProductionRule> finalProductions(vector<string> rawProduction);
    pair<string,vector<string>> getNameAndProductions(string rawProduction);
    pair<string,int> getProductionName(string rawProduction);
    vector<string> getRawORProductions(string combineProduction);
    vector<ParserToken> getTokens(string production);

};


#endif //JAVACOMPILER_PRODUCTION_GENERATOR_H
