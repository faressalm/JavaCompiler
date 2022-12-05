//
// Created by Fares Salm on 12/5/2022.
//
#include "lexical_analyzer.h"
#include <bits/stdc++.h>
using namespace std;

LexicalAnalyzer::LexicalAnalyzer(string inputFilePath){ inputFile.open(inputFilePath);}
char LexicalAnalyzer::getNextChar(){
    if(inputLineIndex == inputLine.size())
        if(getline(inputFile, inputLine))
            inputLineIndex = 0;
        else{
            inputFile.close();
            return ' ';
        }
    if(skipCharacters.find(inputLine[inputLineIndex]) != skipCharacters.end()){
        inputLineIndex++;
        return getNextChar();
    }
    return inputLine[inputLineIndex++];
}
pair<string,string> LexicalAnalyzer::getNextToken(){
    string rawToken = "";
    char nextChar;
    pair<string,string> nameAndValue;
    while((nextChar = getNextChar()) != ' '){
        rawToken += nextChar;
        cout<<rawToken<<endl;
        return {rawToken,"int"};
    }
    if(nextChar == ' ')
        return {"",""}; // input file has finished
    return   nameAndValue;
}