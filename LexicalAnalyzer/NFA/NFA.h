#ifndef COMPILER_NFA_H
#define COMPILER_NFA_H

#include <bits/stdc++.h>

using namespace std;

const string eps = "\\L";


class State{
public:
    static int id_generator;
    unordered_map<string, vector<State*>> transitions;
    const int id;

    State();

    void addTransition(const string& label, State* next);
    unordered_set<State*> move(unordered_set<State*> &nodes, string label);
    unordered_set<State*> e_closure(unordered_set<State* > &nodes);
};


namespace std
{
    template<>struct hash< unordered_set<State*> >{
        size_t operator() (const unordered_set<State*>& st) const noexcept {
            size_t res = 0;
            auto op = hash<unordered_set<State*>::value_type>{};
            for(const auto& elem: st){
                res ^= op(elem);
            }
            return res;
        }
    };
}
class NFA{
public:
    State* start;
    State* end;
    unordered_map<State*, pair<string, int>> acceptingStates; // node > (Token_Name, priority)

    explicit NFA(const string& label);
    NFA(State* start, State* end, unordered_map<State*, pair<string, int>> acceptingStates);
    NFA();
    set<string> chars;
};

/** Builder functions **/
class NFA_builder {
public:
    NFA_builder();

    NFA build(const vector<queue<pair<string, bool>>>& REs, const vector<pair<string, int>>& tokens);
    NFA build_nfa(queue<pair<string, bool>> re, const pair<string, int>& token);
    NFA Or(NFA &a, NFA &b);
    NFA concatenate(NFA &a, NFA &b);
    NFA positive_closure(NFA &a);
    NFA kleene_closure(NFA &a);
    NFA hyphen(NFA &a, NFA &b);
};

#endif //COMPILER_NFA_H