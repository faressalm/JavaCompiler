//
// Created by Fares Salm on 12/22/2022.
//

#include "production_generator.h"

ProductionGenerator::ProductionGenerator(string productionsPath):productionsPath(productionsPath){}

vector<ProductionRule> ProductionGenerator::getProductions() {
    ifstream file(productionsPath);
    string productionLine;
    string buffer="";
    if (file.is_open()){
        while (getline(file, productionLine))
            buffer+=productionLine;
        file.close();
    }
    return finalProductions(getRawProductions(buffer));
}

unordered_set<string> ProductionGenerator::getSetOfTerminals(vector<ProductionRule> productionRules){
    unordered_set<string> terminals;
    for(auto &productionRule: productionRules)
        for(auto & rule: productionRule.rules)
            for(auto &token : rule)
                if(token.type == ParserToken::Terminal)
                    terminals.insert(token.name);
    return terminals;
}

vector<string> ProductionGenerator::getRawProductions(string buffer){
    vector<int> startIndexes;
    for(int i=0;i<buffer.size();i++)
        if(buffer[i]=='#')
            startIndexes.push_back(i);
    startIndexes.push_back(buffer.size());

    vector<string> rawProductions;
    for(int i=0;i<startIndexes.size()-1;i++)
        rawProductions.push_back(buffer.substr(startIndexes[i]+1,startIndexes[i+1]-startIndexes[i]-1));
    return rawProductions;
}

vector<ProductionRule> ProductionGenerator::finalProductions(vector<string> rawProductions){
    vector<ProductionRule> productionRules;
    for (auto & rawProduction: rawProductions){
        pair<string,vector<string>>  nameAndProductions = getNameAndProductions(rawProduction);
        vector<vector<ParserToken>> rules;
        for(auto & production:nameAndProductions.second)
            rules.push_back(getTokens(production));
        productionRules.push_back(ProductionRule(nameAndProductions.first,rules));
    }
    return productionRules;
}

pair<string,vector<string>> ProductionGenerator::getNameAndProductions(string rawProduction){
    pair<string,int> nameAndStart = getProductionName(rawProduction);
    vector<string> productions = getRawORProductions(rawProduction.substr(nameAndStart.second));
    return {nameAndStart.first, productions};
}

pair<string,int> ProductionGenerator::getProductionName(string rawProduction){
    int i = 0;
    while(skipCharacters.count(rawProduction[i]) !=0 ) i++;
    string productionName="";
    while (rawProduction[i] != '=') {
        if(rawProduction[i] != ' ')
            productionName+=rawProduction[i];
        i++;
    }
    return {productionName,i+1};
}

vector<string> ProductionGenerator::getRawORProductions(string combineProduction){
    vector<int> startIndexes;
    startIndexes.push_back(0);
    for(int i=1;i<combineProduction.size();i++)
        if(combineProduction[i] =='|'&& combineProduction[i] != '\'' && combineProduction[i] != '\\')
            startIndexes.push_back(i);
    startIndexes.push_back(combineProduction.size());

    vector<string> rawORProductions;
    rawORProductions.push_back(combineProduction.substr(startIndexes[0],startIndexes[1]-startIndexes[0]));
    for(int i=1;i<startIndexes.size()-1;i++)
        rawORProductions.push_back(combineProduction.substr(startIndexes[i]+1,startIndexes[i+1]-startIndexes[i]-1));
    return rawORProductions;
}

vector<ParserToken> ProductionGenerator::getTokens(string production){
    vector<ParserToken> tokens;
    for(int i=0;i<production.size();i++){
        while(i<production.size() && skipCharacters.count(production[i]) != 0) i++;
        if(i == production.size()) break;
        int start = i;
        bool isTerminal = (production[start] == '\'');
        bool isEpsilon = (production[start] == '\\' && production[start+1] == 'L');
        ParserToken::Type tokenType = ParserToken::Epsilon ;
        if(isEpsilon)
            i+=2;
        else if(isTerminal){
            tokenType = ParserToken::Terminal;
            i++;
            while(production[i] != '\'') i++;
            i++;
        }
        else {
            tokenType = ParserToken::NonTerminal;
            while (skipCharacters.count(production[i]) == 0 && production[i] != '\'' && production[start] != '\\') i++;
        }
        int terminalTrunc = isTerminal && !isEpsilon;
        tokens.push_back(ParserToken(tokenType, production.substr(start+terminalTrunc, i-start - int(terminalTrunc) *2)));
    }
    return tokens;
}

vector<ProductionRule> ProductionGenerator::leftFactor(vector<ProductionRule> productionRules, ProductionRule start) {
    for (int i = 0; i < productionRules.size(); i++) {
        vector<ProductionRule> newProductionRule = leftFactorOneRule(productionRules[i]);
        if (newProductionRule.size() == 1)
            continue;

        productionRules.erase(productionRules.begin() + i);
        for (auto newRule : newProductionRule)
            productionRules.push_back(newRule);

        i = -1;
    }

    for (int i = 0; i < productionRules.size(); ++i)
        if (productionRules[i].name == start.name) {
            start = productionRules[i];
            productionRules.erase(productionRules.begin() + i);
            break;
        }

    vector<ProductionRule> newProductionRules;
    newProductionRules.push_back(start);
    for (auto productionRule : productionRules)
        newProductionRules.push_back(productionRule);

    return newProductionRules;
}

vector<ProductionRule> ProductionGenerator::leftFactorOneRule(ProductionRule productionRule) {
    vector<vector<ParserToken>> productions = productionRule.rules;
    vector<ProductionRule> newProductionRule;
    for (int i = 0; i < productions.size(); i++) {
        vector<vector<ParserToken>> match;
        int minShouldMatched = INT_MAX;
        for (int j = i + 1; j < productions.size(); j++) {
            int maxMatched = maxMatchTokens(productions[i], productions[j]);
            if (maxMatched == 0)
                continue;

            if (minShouldMatched > maxMatched)
                minShouldMatched = maxMatched;

            match.push_back(productions[j]);
            productions.erase(productions.begin() + j);
            j = i;
        }

        match.push_back(productions[i]);
        if (match.size() == 1)
            continue;

        vector<ParserToken> common;
        for (int j = 0; j < minShouldMatched; j++)
            common.push_back(productions[i][j]);

        string newName = productionRule.name + "_DASH";
        common.push_back(ParserToken(ParserToken::NonTerminal, newName));
        productions.erase(productions.begin() + i);
        productions.push_back(common);
        vector<vector<ParserToken>> newMatch;
        for (auto temp : match) {
            for (int k = 0; k < minShouldMatched; k++)
                temp.erase(temp.begin());

            if (temp.empty()) {
                vector<ParserToken> eps;
                eps.push_back(ParserToken(ParserToken::Epsilon, "\\L"));
                newMatch.push_back(eps);
            } else {
                newMatch.push_back(temp);
            }
        }

        newProductionRule.push_back(ProductionRule(newName, newMatch));
        i = -1;
    }

    newProductionRule.push_back(ProductionRule(productionRule.name, productions));

    return newProductionRule;
}

int ProductionGenerator::maxMatchTokens(vector<ParserToken> a, vector<ParserToken> b) {
    int i = 0;
    for (; i < min(a.size(), b.size()); i++)
        if (a[i].name != b[i].name)
            break;

    return i;
}

vector<ProductionRule> ProductionGenerator::eliminateLR(vector<ProductionRule> productionRules, ProductionRule start) {
    for (int i = 0; i < productionRules.size(); i++) {
        vector<vector<ParserToken>> productions = productionRules[i].rules;
        for (int j = 0; j < i; j++) {
            for (int k = 0; k < productions.size(); k++) {
                if (productions[k][0].name == productionRules[j].name) {
                    productions[k].erase(productions[k].begin());
                    vector<vector<ParserToken>> jProductions = productionRules[j].rules;
                    for (auto& jProduction : jProductions)
                        for (auto m : productions[k])
                            jProduction.push_back(m);

                    productions.erase(productions.begin() + k);
                    for (auto jProduction : jProductions)
                        productions.push_back(jProduction);
                }
            }
        }

        productionRules[i] = ProductionRule(productionRules[i].name, productions);
        vector<ProductionRule> newProductionRule = eliminateLROneRule(productionRules[i]);
        if (newProductionRule.size() == 1)
            continue;

        vector<ProductionRule> temp;
        for (int j = 0; j < i; j++)
            temp.push_back(productionRules[j]);

        for (auto newRule : newProductionRule)
            temp.push_back(newRule);

        for (int j = i + 1; j < productionRules.size(); j++)
            temp.push_back(productionRules[j]);

        productionRules = temp;
        i = -1;
    }

    for (int i = 0; i < productionRules.size(); i++)
        if (productionRules[i].name == start.name) {
            start = productionRules[i];
            productionRules.erase(productionRules.begin() + i);
            break;
        }

    vector<ProductionRule> newProductionRules;
    newProductionRules.push_back(start);
    for (auto productionRule : productionRules)
        newProductionRules.push_back(productionRule);

    return newProductionRules;
}

bool epsilon = false;
vector<ProductionRule> ProductionGenerator::eliminateLROneRule(ProductionRule productionRule) {
    vector<ProductionRule> newProductionRules;
    if (!isLeftRecursion(productionRule)) {
        newProductionRules.push_back(productionRule);
        return newProductionRules;
    }

    string name = productionRule.name;
    vector<vector<ParserToken>> productions = productionRule.rules;
    epsilon = false;
    pair<vector<vector<ParserToken>>, vector<vector<ParserToken>>> nonLRandLRProductions = getNonLRandLR(productionRule);
    vector<vector<ParserToken>> nonLRProductions = nonLRandLRProductions.first;
    vector<vector<ParserToken>> LRProductions = nonLRandLRProductions.second;
    string newName = name + "_DASH";
    ParserToken newToken = ParserToken(ParserToken::NonTerminal, newName);
    vector<vector<ParserToken>> newNonTerminal;
    for (auto LRProduction : LRProductions) {
        LRProduction.push_back(newToken);
        newNonTerminal.push_back(LRProduction);
    }

    if (!nonLRProductions.empty() || epsilon) {
        vector<ParserToken> epsilonToken;
        epsilonToken.push_back(ParserToken(ParserToken::Epsilon, "\\L"));
        newNonTerminal.push_back(epsilonToken);
    }

    productions.clear();
    for (auto nonLRProduction : nonLRProductions) {
        nonLRProduction.push_back(newToken);
        productions.push_back(nonLRProduction);
    }

    if (nonLRProductions.empty()) {
        vector<ParserToken> temp;
        temp.push_back(newToken);
        productions.push_back(temp);
    }

    newProductionRules.push_back(ProductionRule(productionRule.name, productions));
    newProductionRules.push_back(ProductionRule(newName, newNonTerminal));

    return newProductionRules;
}

bool ProductionGenerator::isLeftRecursion(ProductionRule productionRule) {
    string name = productionRule.name;
    vector<vector<ParserToken>> productions = productionRule.rules;
    for (auto production : productions)
        if (production[0].name == name)
            return true;

    return false;
}

pair<vector<vector<ParserToken>>, vector<vector<ParserToken>>>
ProductionGenerator::getNonLRandLR(ProductionRule productionRule) {
    string name = productionRule.name;
    vector<vector<ParserToken>> productions = productionRule.rules;
    vector<vector<ParserToken>> nonLR, LR;
    for (auto production : productions) {
        if (production[0].name != name) {
            if (production[0].name != "\\L")
                nonLR.push_back(production);
            else
                epsilon = true;
        } else {
            production.erase(production.begin());
            LR.push_back(production);
        }
    }

    return make_pair(nonLR, LR);
}
