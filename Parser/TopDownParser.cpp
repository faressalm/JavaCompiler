//
// Created by Fares Salm on 12/29/2022.
//

#include "TopDownParser.h"

TopDownParser::TopDownParser(ParserUtils &parserUtils, ParserToken startToken):parserUtils(parserUtils),startToken(startToken){}

void TopDownParser::parse(){
    LexicalAnalyzer lexicalAnalyzer = lexical();
    ofstream outputFile("..\\parserOutput.txt", std::ofstream::out);
    stack<ParserToken> parserTokens;
    queue<ParserToken> lexicalTokens;
    ParserToken end_symbol = ParserToken(ParserToken::Terminal,"$");
    parserTokens.push(end_symbol);
    parserTokens.push(startToken);
    bool endTokens = false;
    while(!parserTokens.empty() && !endTokens){
        if(lexicalTokens.empty())
            if(lexicalAnalyzer.fileClosed)
                lexicalTokens.push(ParserToken(ParserToken::Terminal, "$"));
            else lexicalTokens.push(ParserToken(ParserToken::Terminal,lexicalAnalyzer.getNextToken().first));
        ParserToken topParserToken = parserTokens.top();
        if(topParserToken.isTerminal() && topParserToken.name == lexicalTokens.front().name){
            outputFile << "Match: "<< lexicalTokens.front().name<<endl;
            if(lexicalTokens.front().name == "$")
                endTokens = true;
            lexicalTokens.pop();
            parserTokens.pop();
        }else if (topParserToken.isTerminal() && topParserToken.name != lexicalTokens.front().name){
            outputFile << "Error: missing "<< topParserToken.name<< ", inserted"<<endl;
            parserTokens.pop();
        }else {
            pair<string, vector<ParserToken>> parserEntry = parserUtils.get_entry(topParserToken,lexicalTokens.front());
            if(parserEntry.first == "production"){
                outputFile << topParserToken.name << " → ";
                parserTokens.pop();
                for(int i=parserEntry.second.size()-1;i>=0;i--){
                    if(!parserEntry.second[i].isEpsilon())
                        parserTokens.push(parserEntry.second[i]);
                }
                outputFile << parserUtils.get_production_name(parserEntry.second,false)<<endl;
            }else if (parserEntry.first == "sync"){
                outputFile << "Sync: discard "<< topParserToken.name<<endl;
                parserTokens.pop();
            }else {
                outputFile << "Empty: discard "<< lexicalTokens.front().name<<endl;
                if(lexicalTokens.front().name == "$")
                    endTokens = true;
                lexicalTokens.pop();
            }
        }
    }
    outputFile.close();
}

LexicalAnalyzer TopDownParser::lexical(){
    // rule parsing
    pair<vector<pair<string,int>>, vector<queue<pair<string,bool>>>> REs  =
            LexicalRulesGenerator("..\\lexical_rules.txt").generateNFAs();
    // get NFA
    NFA startNFA =  NFA_builder().build(REs.second,REs.first);
    //get DFA
    DFA dfa = DFA_builder::build_dfa(startNFA);;
    dfa.transition_table( "..\\Transition_Table.csv",startNFA.chars);
    //parse program file
    cout << "Number of states after minimization: "<<dfa.states.size() <<endl<<"=========================================================="<<endl;
    return  LexicalAnalyzer("..\\testprogram.txt",dfa,dfa.states[0]);
}