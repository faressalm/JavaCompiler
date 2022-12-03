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
    // "." stands for concatenation
    unordered_map<string,int> fixedRulesChars = { {".",5},{"-",10}, {"|",5}, {"+",8}, {"(",0}, {")",0}, {"*",8}};
public:
    LexicalAnalyzerGenerator(string lexicalRulesPath): lexicalRulesPath(lexicalRulesPath){}
    vector<NFA> generateNFAs(){
        ifstream file(lexicalRulesPath);
        string lexicalRuleLine;
        vector<string> keywords;
        vector<string> punctuations;
        vector<pair<string,string>> RDs;
        vector<pair<string,queue<pair<string,bool>>>> REsPostfixs;
        vector<pair<string,string>> REs;

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
                        REs.push_back({tokenNameAndAssignIndex.first, expression});
                        REsPostfixs.push_back({tokenNameAndAssignIndex.first, getPostFix(expression)});
                    }

                }
            }
            file.close();
        }
        for(auto &p:REs)
            cout<<p.first<<" : "<<p.second<<endl;
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
    queue<pair<string,bool>> getPostFix(string expression){
        queue<pair<string,bool>> postfix;
        string nextChar;
        stack<string> operators;
        string  prevChar = "-"; // to check for the concatenation operator condition
        bool concatenateFound = false;
        bool wasOperator = false;
        for (int i = 0; i < expression.size() ; i++) {
            if(expression[i] == ' ')
                continue;
            nextChar = expression[i];
            if(nextChar == "\\")
                nextChar = (expression[++i] == 'L')? "\\L" : ""+expression[i];
            string endChar = "+*)";
            cout<<prevChar<<nextChar << endl;
            if(((!isOperator(prevChar)&&!wasOperator) || endChar.find(prevChar) != string::npos) && (!isOperator(nextChar)|| nextChar == "(") && prevChar != nextChar){ //check for concatenation
                i--;
                nextChar = ".";
                wasOperator = true;
                concatenateFound = true;
            }else
                wasOperator = false;
            if(!isOperator(nextChar)&&!concatenateFound)
                postfix.push({nextChar,false});
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
private:
    bool isOperator (string nextChar) {
        return fixedRulesChars.find(nextChar) != fixedRulesChars.end()  && nextChar != ".";
    }
};

int main() {
    LexicalAnalyzerGenerator lexicalAnalyzerGenerator =  LexicalAnalyzerGenerator("..\\lexical_rules.txt");
//    lexicalAnalyzerGenerator.generateNFAs();
    vector<string> punctuations;
    string ex = "( 0 - 9)+ | ( 0 - 9)+ ( ( 0. 0 - 9)+) ( \\L | E ( ( 0 - 9)+))  ";
    queue<pair<string,bool>> q = lexicalAnalyzerGenerator.getPostFix(ex);
    while (!q.empty()){
        cout<<q.front().first<<" ";
        q.pop();
    }

    return 0;
}
