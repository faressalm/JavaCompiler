//
// Created by Fares Salm on 12/23/2022.
//

#ifndef JAVACOMPILER_PARSER_TOKEN_H
#define JAVACOMPILER_PARSER_TOKEN_H
#include <bits/stdc++.h>
using namespace std;

class ParserToken{
public:
    enum Type{
        Terminal,
        NonTerminal,
        Epsilon
    };
    Type type;
    string name;
    ParserToken(Type type,string name);
    bool operator==(const ParserToken &a) const {
        return name == a.name && type == a.type;
    }
    static string typeName(int type);
    bool isTerminal();
    bool isNonTerminal();
    bool isEpsilon();
};

namespace std
{
    template<>
    struct hash<ParserToken>
    {
        size_t
        operator()(const ParserToken &parserToken) const noexcept {
            return hash<string>()(parserToken.name);
        }
    };
}

#endif //JAVACOMPILER_PARSER_TOKEN_H
