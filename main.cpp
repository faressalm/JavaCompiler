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

int main() {
    LexicalAnalyzerGenerator lexicalAnalyzerGenerator =  LexicalAnalyzerGenerator("..\\lexical_rules.txt");
    lexicalAnalyzerGenerator.generateNFAs();

    return 0;
}
