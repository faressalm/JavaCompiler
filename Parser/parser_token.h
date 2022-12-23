//
// Created by Fares Salm on 12/23/2022.
//

#ifndef JAVACOMPILER_PARSER_TOKEN_H
#define JAVACOMPILER_PARSER_TOKEN_H
#include <bits/stdc++.h>
using namespace std;

class ParserToken {
public:
    enum Type{
        Terminal,
        NonTerminal,
        Epsilon
    };
    Type type;
    string name;
    ParserToken(Type type,string name);
    static string typeName(int type);
};


#endif //JAVACOMPILER_PARSER_TOKEN_H
