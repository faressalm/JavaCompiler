#include <iostream>
#include <bits/stdc++.h>
using namespace std;
class State{
    public:
        int index;
        int priority;
        bool isAccepting;
        unordered_map<string,vector<State*>> edges;
        State(int index): index(index){
            priority = 0;
            isAccepting = false;
        }
};
class NFA{
    public:
        State* startState;
        State* endState;
        int totalNumberOfStates;
        NFA(int totalNumberOfStates,State* startState,State* endState):
                totalNumberOfStates(totalNumberOfStates), endState(endState), startState(startState){}
};

class LexicalAnalyzerGenerator{
private:
    string lexicalRulesPath;
    unordered_set<char> fixedRulesChars = {'-','|','+','(',')','*'};
public:
    LexicalAnalyzerGenerator(string lexicalRulesPath): lexicalRulesPath(lexicalRulesPath){}
    vector<NFA> generateNFAs(){
        ifstream file(lexicalRulesPath);
        string lexicalRuleLine;
        vector<string> keywords;
        vector<string> punctuations;
        vector<pair<string,string>> RDs;
        vector<pair<string,queue<string>>> REsPostfixs;
        if (file.is_open()){
            while (getline(file, lexicalRuleLine))
            {
                if(lexicalRuleLine[0]!='{')
                    addKeywords(keywords,lexicalRuleLine);
                else if(lexicalRuleLine[0]!='[')
                    addPunctuations(punctuations,lexicalRuleLine);
                else{
                    pair<string,int> tokenNameAndAssignIndex = getTokenName(lexicalRuleLine);
                    string expression = lexicalRuleLine.substr(tokenNameAndAssignIndex.second+1);
                    bool isRD = (lexicalRuleLine[tokenNameAndAssignIndex.second] == '=');
                    replaceRDs(expression,RDs);
                    if(isRD)
                        RDs.push_back({tokenNameAndAssignIndex.first,expression});
                    else
                        REsPostfixs.push_back({tokenNameAndAssignIndex.first, getPostFix(expression)});

                }
            }
            file.close();
        }
        return {};
    }
//private:
    //assume each keyword is separated from others with spaces
    void addKeywords(vector<string> &keywords,string & lexicalRuleLine){
        int lineLength = lexicalRuleLine.size();
        string endChars = " /n/t}";
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
    void addPunctuations(vector<string> &punctuations,string & lexicalRuleLine){
        int lineLength = lexicalRuleLine.size();
        string punctuation;
        for(int i=1;i<lineLength-1;i++){
            if(lexicalRuleLine[i]==' ')
                continue;
            punctuation = (lexicalRuleLine[i]=='\\')? lexicalRuleLine[++i] : lexicalRuleLine[i];
            punctuations.push_back(punctuation);
        }
    }
    /** first -> token name, second-> index of assign operator for RE or RD*/
    pair<string,int> getTokenName(string lexicalRuleLine){
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
    // assume no Expression will be defined  with RD before this RD is defined
    // assume RDs name will if one's name is part of other one the smaller will be defined first
    void replaceRDs(string &expression,vector<pair<string,string>> &RDs){
        int index;
        for(int i= RDs.size()-1;i>=0;i--){
            while((index = expression.find(RDs[i].first)) != string::npos) {    //for each location where RD name is found
                expression.replace(index, RDs[i].first.length(), "("+RDs[i].second+")"); //remove and replace from that position
            }
        }
    }
    queue<string> getPostFix(string expression){
        queue<string> postfix;
        for (int i = 0; i < expression.size() ; ++i) {
            while (expression[i]==' ') i++;
        }
    }


};

int main() {
    LexicalAnalyzerGenerator lexicalAnalyzerGenerator =  LexicalAnalyzerGenerator("..\\lexical_rules.txt");
//    lexicalAnalyzerGenerator.generateNFAs();
    vector<string> punctuations;
    string ex = " letter (letter|digit)* digit digits";
    vector<pair<string,string>> RDs= {{"letter","a-z | A-Z"},{"digit","0 - 9"},{"digits","(0 - 9)+"}};
    lexicalAnalyzerGenerator.replaceRDs(ex,RDs);
    cout<<ex;
    return 0;
}
