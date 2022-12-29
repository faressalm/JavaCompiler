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
        cout<<"1"<<endl;
        if(lexicalTokens.empty())
            if(lexicalAnalyzer.fileClosed)
                lexicalTokens.push(ParserToken(ParserToken::Terminal, "$"));
            else lexicalTokens.push(ParserToken(ParserToken::Terminal,lexicalAnalyzer.getNextToken().first));
        cout<<"2"<<endl;

        ParserToken topParserToken = parserTokens.top();
        cout<<"3"<<endl;

        if(topParserToken.isTerminal() && topParserToken.name == lexicalTokens.front().name){
            cout<<"4"<<endl;
            outputFile << "Match: "<< lexicalTokens.front().name<<endl;
            cout << "Match: "<< lexicalTokens.front().name<<endl;
            cout<<"5"<<endl;
            if(lexicalTokens.front().name == "$")
                endTokens = true;
            cout<<"6"<<endl;
            lexicalTokens.pop();
            parserTokens.pop();
            cout<<"7"<<endl;
        }else if (topParserToken.isTerminal() && topParserToken.name != lexicalTokens.front().name){
            cout<<"8"<<endl;
            outputFile << "Error: missing "<< topParserToken.name<< ", inserted"<<endl;
            cout << "Error: missing "<< topParserToken.name<< ", inserted"<<endl;
            cout<<"9"<<endl;
            parserTokens.pop();
            cout<<"10"<<endl;
        }else {
            cout<<"11"<<endl;
            pair<string, vector<ParserToken>> parserEntry = parserUtils.get_entry(topParserToken,lexicalTokens.front());
            cout<<"12"<<endl;
            if(parserEntry.first == "production"){
                cout<<"13"<<endl;
                outputFile << topParserToken.name << " → ";
                parserTokens.pop();
                for(int i=parserEntry.second.size()-1;i>=0;i--){
                    if(!parserEntry.second[i].isEpsilon())
                        parserTokens.push(parserEntry.second[i]);
                }
                cout<<"14"<<endl;
                outputFile << parserUtils.get_production_name(parserEntry.second,false)<<endl;
                cout << parserUtils.get_production_name(parserEntry.second, false)<< " "<< lexicalTokens.front().name<<endl;
            }else if (parserEntry.first == "sync"){
                cout<<"15"<<endl;
                outputFile << "Sync: discard "<< topParserToken.name<<endl;
                cout << "Sync: discard "<< topParserToken.name<<endl;
                cout<<"16"<<endl;
                parserTokens.pop();
                cout<<"17"<<endl;
            }else {
                cout<<"18"<<endl;
                outputFile << "Empty: discard "<< lexicalTokens.front().name<<endl;
                cout << "Empty: discard "<< lexicalTokens.front().name<<endl;
                if(lexicalTokens.front().name == "$")
                    endTokens = true;
                lexicalTokens.pop();
                cout<<"19"<<endl;
            }
            cout<<"20"<<endl;
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