#include "NFA.h"
#include <bits/stdc++.h>

#include <utility>
using namespace std;

set<string> labels;

int State::id_generator = 0;

State::State(): id(id_generator++) {}

void State::addTransition(const string& label, State *next) {
    (this->transitions)[label].push_back(next);
}

NFA::NFA(){
    this->start = new State();
    this->end = new State();
    this->start->addTransition(eps, this->end);
}

NFA::NFA(const string& label) {
    this->start = new State();
    this->end = new State();
    this->start->addTransition(label, this->end);
}

NFA::NFA(State *start, State *end, unordered_map<State *, pair<string, int>> acceptingStates) {
    this->start = start;
    this->end = end;
    this->acceptingStates = std::move(acceptingStates);
}
NFA_builder::NFA_builder(){

}

void combine(unordered_map<State *, pair<string, int>>& acceptingStates,
             unordered_map<State *, pair<string, int>>& acceptingStatesT){
    auto itr = acceptingStatesT.begin();
    while(itr != acceptingStatesT.end()){
        acceptingStates[itr->first] = itr->second;
        itr++;
    }
}

NFA NFA_builder::build(const vector<queue<pair<string, bool>>>& REs, const vector<pair<string, int>>& tokens) {
    if( REs.size() == 0) {
        throw logic_error("Size of the queue can't be empty");
    }
    labels.clear();
    NFA c = build_nfa(REs[0], tokens[0]);
    for(int i = 1 ; i < (int) REs.size(); i++){
        NFA a = build_nfa(REs[i], tokens[i]);
        c = Or(c, a);
    }
    c.chars = labels;
    if( c.chars.find(eps) != c.chars.end()){
        c.chars.erase(eps);
    }
    return c;
}

NFA NFA_builder::build_nfa(queue<pair<string, bool>> re, const pair<string, int>& token) {
    stack<NFA> NFAs;
    while (!re.empty()){
        pair<string, bool> label = re.front();
        re.pop();
        if(label.second){ // Operator
            NFA b = NFAs.top();
            NFAs.pop();
            if(label.first == "|"){
                NFA a = NFAs.top();
                NFAs.pop();
                NFAs.push(Or(a , b));
            }
            else if (label.first == "."){
                NFA a = NFAs.top();
                NFAs.pop();
                NFAs.push(concatenate(a , b));
            }
            else if(label.first == "*"){
                NFAs.push(kleene_closure(b));
            }
            else if(label.first == "+"){
                NFAs.push(positive_closure(b));
            }
            else if(label.first == "-"){
                NFA a = NFAs.top();
                NFAs.pop();
                NFAs.push(hyphen(a , b));
            }
        }
        else{
            labels.insert(label.first);
            NFAs.push(NFA(label.first)); // A normal label
        }
    }
    if(NFAs.size() != 1 ){
        cout << NFAs.size() << "\n";
        throw logic_error("Stack should have one NFA at the end");
    }
    NFA ret = NFAs.top();
    ret.acceptingStates[ret.end] = token;
    return ret;
}
NFA NFA_builder::Or(NFA &a, NFA &b) {
    State* new_start = new State(), *new_end = new State();
    new_start->addTransition(eps, a.start);
    new_start->addTransition(eps, b.start);
    a.end->addTransition(eps,new_end);
    b.end->addTransition(eps,new_end);
    combine(a.acceptingStates, b.acceptingStates);

    return NFA(new_start, new_end, a.acceptingStates);
}

NFA NFA_builder::concatenate(NFA &a, NFA &b) {
    a.end->addTransition(eps, b.start);
    a.end = b.end;
    combine(a.acceptingStates, b.acceptingStates);
    return a;
}

NFA NFA_builder::hyphen(NFA &a, NFA &b) {
    char s , e ;
    auto itr = a.start->transitions.begin();
    s = (itr->first)[0];
    itr = b.start->transitions.begin();
    e = (itr->first)[0];
    string label ="" ;
    label.push_back(e);
    NFA c(label);
    labels.insert(label);
    while(s != e){
        label.pop_back();
        label.push_back(s);
        labels.insert(label);
        NFA t(label);
        c = Or(c, t);
        s++;
    }
    return c ;
}

NFA NFA_builder::positive_closure(NFA &a) {
    State* new_start = new State(), *new_end = new State();
    new_start->addTransition(eps, a.start);
    a.end->addTransition(eps, new_end);
    a.end->addTransition(eps, a.start);
    return NFA(new_start, new_end, a.acceptingStates);
}

NFA NFA_builder::kleene_closure(NFA &a) {
    NFA c = positive_closure(a);
    c.start->addTransition(eps, c.end);
    return c;
}

unordered_set<State * > State::e_closure(unordered_set<State * > &nodes) {
    unordered_set<State *> result;
    queue<State *> q;
    for(auto &node : nodes){
        q.push(node);
    }
    while(!q.empty()){
        State* node = q.front();
        q.pop();
        auto itr = node->transitions.find(eps);
        if(itr == node->transitions.end()){
            continue;
        }
        for(int i = 0 ; i < (int) (itr->second).size() ; i++){
            if(result.find( (itr->second)[i]) == result.end()){
                result.insert(  (itr->second)[i] );
                q.push( (itr->second)[i] );
            }
        }
    }
    for(State* state : nodes ){
        result.insert(state);
    }
    return result;
}
/**
 * Compute the next set of states after perform a transition with a specific label
 * @param nodes
 * @param label
 * @return
 */
unordered_set<State *> State::move(unordered_set<State *> &nodes, string label) {
    unordered_set<State *> result;
    for(auto &node : nodes) {
        auto itr = node->transitions.find(label);
        if (itr == node->transitions.end())
            continue;
        else {
            for (int i = 0; i < (int) (itr->second).size(); i++)
                result.insert((itr->second)[i]);
        }
    }
    return result;
}