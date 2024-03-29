//
// Created by Fares Salm on 12/3/2022.
//
#include "lexical_rules_generator.h"
#include <bits/stdc++.h>
using namespace std;

    LexicalRulesGenerator::LexicalRulesGenerator(string lexicalRulesPath):
            lexicalRulesPath(lexicalRulesPath){}
    pair<vector<pair<string,int>>, vector<queue<pair<string,bool>>>> LexicalRulesGenerator::generateNFAs(){
        ifstream file(lexicalRulesPath);
        string lexicalRuleLine;
        vector<string> keywords;
        vector<string> punctuations;
        vector<pair<string,string>> RDs;
        vector<queue<pair<string,bool>>> REsPostfixs;
        vector<pair<string,int>> REs;
        int priority  = 0;
        if (file.is_open()){

            while (getline(file, lexicalRuleLine))
            {
                if(lexicalRuleLine[0]=='{')
                    addKeywords(keywords,lexicalRuleLine);
                else if(lexicalRuleLine[0]=='[')
                    addPunctuations(punctuations,lexicalRuleLine);
                else{
                    pair<string,int> tokenNameAndAssignIndex = getTokenName(lexicalRuleLine);
                    string expression = lexicalRuleLine.substr(tokenNameAndAssignIndex.second+1);
                    bool isRD = lexicalRuleLine[tokenNameAndAssignIndex.second] == '=';
                    replaceRDs(expression,RDs);
                    if(isRD)
                        RDs.push_back({tokenNameAndAssignIndex.first,expression});
                    else{
                        REs.push_back({tokenNameAndAssignIndex.first,0});
                        REsPostfixs.push_back(getPostFix(expression));
                    }

                }
            }
            file.close();
        }
        for(int i = REs.size()-1;i>=0;i--)
            REs[i].second = priority++;
        for(auto &keyword:keywords)
        {
            REs.push_back({keyword,priority++});
            REsPostfixs.push_back(getPostFix(keyword));
        }
        for(auto &punctuation:punctuations)
        {
            REs.push_back({punctuation.size()==2? punctuation.substr(1):punctuation,priority++});
            REsPostfixs.push_back(getPostFix(punctuation));
        }
        return {REs,REsPostfixs};
    }

    void LexicalRulesGenerator::addKeywords(vector<string> &keywords, string & lexicalRuleLine){
        int lineLength = lexicalRuleLine.size();
        string endChars = " \n\t}";
        string keyword = "";

        for(int i=1;i<lineLength-1;i++){
            while (lexicalRuleLine[i]==' '&& i<lineLength-1) i++;
            keyword= "";
            while (i<lineLength && endChars.find(lexicalRuleLine[i]) == string::npos)
                keyword+=lexicalRuleLine[i++];
            if(!keyword.empty())
                keywords.push_back(keyword);
        }
    }

    void LexicalRulesGenerator::addPunctuations(vector<string> &punctuations, string & lexicalRuleLine){
        int lineLength = lexicalRuleLine.size();
        string punctuation;
        for(int i=1;i<lineLength-1;i++){
            if(lexicalRuleLine[i]==' ')
                continue;
            punctuation = (lexicalRuleLine[i]=='\\')? lexicalRuleLine.substr(i++,2) : lexicalRuleLine.substr(i,1);
            punctuations.push_back(punctuation);
        }
    }

    /** first -> token name, second-> index of assign operator for RE or RD*/
    pair<string,int> LexicalRulesGenerator::getTokenName(string lexicalRuleLine){
        int i = 0;
        while (lexicalRuleLine[i]==' ') i++;
        string tokenName="";
        while (lexicalRuleLine[i]!='='&&lexicalRuleLine[i]!=':') {
            if(lexicalRuleLine[i]!=' ')
                tokenName+=lexicalRuleLine[i];
            i++;
        }
        return {tokenName,i};
    }

    /** replace RDs in RE and RD with it only Terminals*/
    void LexicalRulesGenerator::replaceRDs(string &expression, vector<pair<string,string>> &RDs){
        int index;
        for(int i= RDs.size()-1;i>=0;i--){
            while((index = expression.find(RDs[i].first)) != string::npos) {    //for each location where RD name is found
                expression.replace(index, RDs[i].first.length(), "("+RDs[i].second+")"); //remove and replace from that position
            }
        }
    }

    queue<pair<string,bool>> LexicalRulesGenerator::getPostFix(string expression){
        queue<pair<string,bool>> postfix;
        string nextChar;
        stack<string> operators;
        string  prevChar = "start"; // to check for the concatenation operator condition
        bool concatenateFound = false;
        bool wasOperator = false;
        string endChar = "+*)";
        for (int i = 0; i < expression.size() ; i++) {
            if(expression[i] == ' ')
                continue;
            nextChar = expression.substr(i,1);
            if(nextChar == "\\"){
                nextChar = (expression[++i] == 'L')? "\\L" : expression.substr(i-1,2);
            }
            if(((!isOperator(prevChar)&&!wasOperator) || endChar.find(prevChar) != string::npos) && (!isOperator(nextChar)|| nextChar == "(")){ //check for concatenation
                i-= nextChar.size();
                nextChar = ".";
                wasOperator = true;
                concatenateFound = true;
            }else
                wasOperator = false;
            if(!isOperator(nextChar)&&!concatenateFound)
                postfix.push({nextChar.size()==2&& nextChar[1]!='L'? nextChar.substr(1): nextChar,false});
            else{
                if(nextChar == "(")
                    operators.push(nextChar);
                else if (nextChar == ")")
                {
                    while(!operators.empty()&& operators.top() != "(")
                    {
                        postfix.push({operators.top(),true});
                        operators.pop();
                    }
                    operators.pop();
                }else{ // operator
                    while(!operators.empty()&& fixedRulesChars[operators.top()] >= fixedRulesChars[nextChar])
                    {
                        postfix.push({operators.top(),true});
                        operators.pop();
                    }
                    operators.push(nextChar);
                }
            }
            prevChar =nextChar;
            concatenateFound = false;
        }
        // get all the rest of operators to the queue
        while(!operators.empty())
        {
            postfix.push({operators.top(),true});
            operators.pop();
        }
        return postfix;
    }

    bool LexicalRulesGenerator::isOperator (string nextChar) {
        return fixedRulesChars.find(nextChar) != fixedRulesChars.end()  && nextChar != ".";
    }