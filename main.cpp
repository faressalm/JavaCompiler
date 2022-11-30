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
    unordered_map<string,queue<string>> RegularDefsPostfixs;
    unordered_set<char> fixedRulesChars = {'-','|','+','(',')','*'};
public:
    LexicalAnalyzerGenerator(string lexicalRulesPath): lexicalRulesPath(lexicalRulesPath){}
    vector<NFA> generateNFAs(){
        ifstream file(lexicalRulesPath);
        string lexicalRuleLine;
        vector<string> keywords;
        vector<string> punctuations;
        if (file.is_open()){
            while (getline(file, lexicalRuleLine))
            {
                if(lexicalRuleLine[0]!='{')
                    addKeywords(keywords,lexicalRuleLine);
                else if(lexicalRuleLine[0]!='[')
                    addPunctuations(punctuations,lexicalRuleLine);
                else{
                    pair<string,int> tokenNameAndAssignIndex = getTokenName(lexicalRuleLine);
                    bool isRE = (lexicalRuleLine[tokenNameAndAssignIndex.second] == '=');

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

    queue<string> getPostFix(string lexicalRuleLine,int startIndex){
        queue<string> postfix;
        for (int i = startIndex + 1; i < lexicalRuleLine.size() ; ++i) {
            while (lexicalRuleLine[i]==' ') i++;
        }
    }


};

int main() {
    LexicalAnalyzerGenerator lexicalAnalyzerGenerator =  LexicalAnalyzerGenerator("..\\lexical_rules.txt");
//    lexicalAnalyzerGenerator.generateNFAs();
    vector<string> punctuations;
    string lex = "[;, \\( \\) { }]";
    lexicalAnalyzerGenerator.addPunctuations(punctuations,lex);
    for (int i = 0; i <punctuations.size() ; ++i) {
        cout<<punctuations[i]<<endl;
    }
    return 0;
}
