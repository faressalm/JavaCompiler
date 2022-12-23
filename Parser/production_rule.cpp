//
// Created by Fares Salm on 12/23/2022.
//

#include "production_rule.h"
ProductionRule::ProductionRule(string name,vector<vector<ParserToken>> rules): name(name), rules(rules){
    for(auto & rule: rules){
        for(int i=0; i<rule.size();i++){
            if(rule[i].type == ParserToken::Epsilon){
                haveEpsilon = true;
                epsilonIndex = i;
            }
        }
    }
}