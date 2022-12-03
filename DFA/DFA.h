#ifndef JAVACOMPILER_DFA_H
#define JAVACOMPILER_DFA_H

#include "../NFA/NFA.h"

class DFA {
public:
    State* start;
    unordered_map<State*, pair<string, int>, MyHashFunction> acceptingStates; // node > (Token_Name, priority)

    DFA();
};

class DFA_builder {
public:
    DFA build_dfa(NFA);
};

#endif //JAVACOMPILER_DFA_H
