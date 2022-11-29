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
    unordered_map<string,NFA*> RegularDefs;
    unordered_set<char> fixedRulesChars = {'-','|','+','(',')','*'};
public:
    LexicalAnalyzerGenerator(string lexicalRulesPath): lexicalRulesPath(lexicalRulesPath){}
    vector<NFA> generateNFAs(){
        ifstream file(lexicalRulesPath);
        string lexicalRuleLine;
        if (file.is_open()){
            while (getline(file, lexicalRuleLine))
            {   if(lexicalRuleLine[0]!='{' && lexicalRuleLine[0]!='[')
                    getNFAFromRE(lexicalRuleLine);
            }
            file.close();
        }
        return {};
    }
private:
    //NFA
    void getNFAFromRE(string lexicalRuleLine){
        pair<string,int> tokenNameAndIndex = getTokenName(lexicalRuleLine);
        bool isRE = (lexicalRuleLine[tokenNameAndIndex.second] == '=');
        for (int i = tokenNameAndIndex.second + 1; i < lexicalRuleLine.size() ; ++i) {
            while (lexicalRuleLine[i]==' ') i++;
        }
        cout<<tokenNameAndIndex.first<<endl;
    }
    // first -> token name, second-> index of assign operator for RE or RD
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
};

int main() {
    LexicalAnalyzerGenerator lexicalAnalyzerGenerator =  LexicalAnalyzerGenerator("..\\lexical_rules.txt");
    lexicalAnalyzerGenerator.generateNFAs();

    return 0;
}
