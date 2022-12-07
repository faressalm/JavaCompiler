#ifndef JAVACOMPILER_DFA_H
#define JAVACOMPILER_DFA_H

#include "../NFA/NFA.h"

class DFA_State {
public:

    int id;
    bool accepting;
    string acceptance_state;
    unordered_map<string, int> transitions;

    DFA_State(int id);
};

class DFA {
public:
    State* start;
    unordered_map<State*, pair<string, int>, MyHashFunction> acceptingStates; // node > (Token_Name, priority)

    vector<DFA_State> states ;

    DFA(vector<DFA_State> states);
    void transition_table(string path);
};

class DFA_builder {
public:
    static DFA build_dfa(NFA);
    static vector<DFA_State> minimize_dfa(vector<DFA_State>);
};

#endif //JAVACOMPILER_DFA_H
