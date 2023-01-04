//
// Created by Fares Salm on 12/23/2022.
//

#include "parser_token.h"

ParserToken::ParserToken(Type type,string name): type(type), name(name){}

string ParserToken::typeName(int type) {
    vector<string> typeNames{"terminal", "nonTerminal", "epsilon"};
    return typeNames[type];
}

bool ParserToken::isTerminal(){
    return ParserToken::Terminal == type;
}
bool ParserToken::isNonTerminal(){
    return ParserToken::NonTerminal == type;
}
bool ParserToken::isEpsilon(){
    return ParserToken::Epsilon == type;
}