//
// Created by Fares Salm on 12/23/2022.
//

#ifndef JAVACOMPILER_PRODUCTION_RULE_H
#define JAVACOMPILER_PRODUCTION_RULE_H
#include <bits/stdc++.h>
#include "parser_token.h"
using namespace std;

class ProductionRule {
public:
    string name;
    bool haveEpsilon;
    int epsilonIndex;
    vector<vector<ParserToken>> rules;
    ProductionRule(string name,vector<vector<ParserToken>> rules);
};


#endif //JAVACOMPILER_PRODUCTION_RULE_H
