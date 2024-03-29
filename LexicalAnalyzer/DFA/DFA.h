#ifndef JAVACOMPILER_DFA_H
#define JAVACOMPILER_DFA_H

#include "../NFA/NFA.h"

class  DFA_State {
public:

    int id;
    bool accepting;
    pair<string,int> acceptance_state;
    unordered_map<string, int> transitions;

    DFA_State(int id);
};

class DFA {
public:
    vector<DFA_State> states ;
    int reject_state; // index of reject state a.k.a empty state.
    DFA(vector<DFA_State> states);
    void transition_table(string path, set<string> chars);
};
void tokenize(string& s, DFA& dfa );
class DFA_builder {
public:
    static DFA build_dfa(NFA);
    static DFA minimize_dfa(vector<DFA_State>, int);
};

#endif //JAVACOMPILER_DFA_H