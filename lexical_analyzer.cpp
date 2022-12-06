//
// Created by Fares Salm on 12/5/2022.
//
#include "lexical_analyzer.h"

LexicalAnalyzer::LexicalAnalyzer(string inputFilePath, DFA dfa,DFA_State startState):dfa(dfa), startState(startState){ inputFile.open(inputFilePath);}
char LexicalAnalyzer::getNextChar(){
    if(fileClosed)
       return ' ';
    if(inputLineIndex == inputLine.size()){
        currentInputLine++;
        if(getline(inputFile, inputLine)){
            inputLineIndex = 0;
            return '\n';
        }
        else {
            fileClosed = true;
            inputFile.close();
            return ' ';
        }
    }
    return inputLine[inputLineIndex++];
}
pair<string,string> LexicalAnalyzer::getNextToken(){
    string rawToken = "";
    char nextChar;
    pair<string,string> nameAndValue;

    stack<DFA_State> maxMunch;
    DFA_State dfaState = startState;
    checkForPrevChar(rawToken,maxMunch,dfaState);

    while(! fileClosed && isSplitChar(nextChar = getNextChar())){cout<<nextChar<<"ss"<<endl;}
    while(! fileClosed && !isSplitChar(nextChar)){
        rawToken += nextChar;
        string charStr (1,nextChar);
        maxMunch.push(dfaState);
        if(dfaState.transitions.find(charStr) != dfaState.transitions.end())
            dfaState = dfa.states[dfaState.transitions[charStr]];
        else {
            cout<<"error found in line: "<<currentInputLine<<", invalid token: "<< rawToken<< nextChar<<endl;
            break;
        }
        nextChar = getNextChar();
        prevChar  = nextChar;
    }
    while(!maxMunch.empty()&&!maxMunch.top().accepting){
        cout<<rawToken<<endl;
        maxMunch.pop();
        rawToken.pop_back();
    }
    if(maxMunch.empty())
        return {"",""};// no token
    else
        return {maxMunch.top().acceptance_state,rawToken};
}
bool LexicalAnalyzer::isSplitChar(char c){
    return splitCharacters.find(c) != splitCharacters.end();
}

void LexicalAnalyzer::checkForPrevChar(string rawToken,stack<DFA_State> & maxMunch,DFA_State &dfaState ){
    if(!isSplitChar(prevChar) && startState.transitions.find(rawToken+prevChar) != startState.transitions.end()){
        rawToken+=prevChar;
        dfaState = dfa.states[dfaState.transitions[rawToken]];
        maxMunch.push(dfaState);
    }
}