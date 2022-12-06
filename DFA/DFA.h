#ifndef JAVACOMPILER_DFA_H
#define JAVACOMPILER_DFA_H

#include "../NFA/NFA.h"
class DFA_State {
public:
    int id;
    bool accepting;
    pair<string, int> acceptance_state;
    unordered_map<string, DFA_State*> transitions;
    bool operator< (const DFA_State &other) const {
        return id < other.id;
    }
};

class DFA {
public:
    State* start;
    unordered_map<State*, pair<string, int>, MyHashFunction> acceptingStates; // node > (Token_Name, priority)

    DFA();
};

class DFA_builder {
public:
    DFA build_dfa(NFA);
    DFA minimize_dfa(vector<DFA_State>);
};

#endif //JAVACOMPILER_DFA_H
