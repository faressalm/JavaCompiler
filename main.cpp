#include <iostream>
#include <bits/stdc++.h>
using namespace std;
/**
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
public:
    LexicalAnalyzerGenerator(string lexicalRulesPath): lexicalRulesPath(lexicalRulesPath){}
    vector<NFA> generateNFAs(){
        ifstream file(lexicalRulesPath);
        string lexicalRuleLine;
        if (file.is_open()){
            while (getline(file, lexicalRuleLine))
            {

            }
            file.close();
        }
        return {};
    }

};
**/
#include "NFA/NFA.h"

int main() {
    //LexicalAnalyzerGenerator lexicalAnalyzerGenerator =  LexicalAnalyzerGenerator("..\\lexical_rules.txt");
    //lexicalAnalyzerGenerator.generateNFAs();
    vector<queue<pair<string,bool>>> REs ;
    vector<pair<string, int>> tokens;
    queue<pair<string,bool>> re ;
    queue<pair<string,bool>> re2 ;
    re.push({"i",false});
    re.push({"n",false});
    re.push({".",true});
    re.push({"t",false});
    re.push({".",true});
    //tokens.push_back({"int", 0});
    //REs.push_back(re);

    re2.push({"a",false});
    //re2.push({"z",false});
    re2.push({"*",true});
    /**
    re2.push({"A",false});
    re2.push({"Z",false});
    re2.push({"-",true});
    re2.push({"|",true});

    re2.push({"a",false});
    re2.push({"z",false});
    re2.push({"-",true});
    re2.push({"A",false});
    re2.push({"Z",false});
    re2.push({"-",true});
    re2.push({"|",true});
    re2.push({"0",false});
    re2.push({"9",false});
    re2.push({"-",true});
    re2.push({"|",true});
    re2.push({"*",true});
    re2.push({".",true});
    **/
    tokens.push_back({"id", 1});
    REs.push_back(re2);
    NFA_builder builder;
    NFA c = builder.build(REs, tokens);



    return 0;
}
