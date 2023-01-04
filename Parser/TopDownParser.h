//
// Created by Fares Salm on 12/29/2022.
//

#ifndef JAVACOMPILER_TOPDOWNPARSER_H
#define JAVACOMPILER_TOPDOWNPARSER_H
#include <bits/stdc++.h>
#include "../LexicalAnalyzer/lexical_analyzer.h"
#include "../LexicalAnalyzer/LexicalRulesParser/lexical_rules_generator.h"
#include "./ParsingTable/parser_utils.h"
using namespace std;

class TopDownParser {
    ParserUtils parserUtils;
    ParserToken startToken;
public:
TopDownParser(ParserUtils &parserUtils, ParserToken startToken);
LexicalAnalyzer lexical();
void parse();
};


#endif //JAVACOMPILER_TOPDOWNPARSER_H
