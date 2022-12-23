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