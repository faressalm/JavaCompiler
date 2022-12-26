//
// Created by Fares Salm on 12/23/2022.
//

#include "parser_token.h"

ParserToken::ParserToken(Type type,string name): type(type), name(name){}

string ParserToken::typeName(int type) {
    vector<string> typeNames{"terminal", "nonTerminal", "epsilon"};
    return typeNames[type];
}

