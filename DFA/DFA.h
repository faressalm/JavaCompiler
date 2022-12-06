#ifndef JAVACOMPILER_DFA_H
#define JAVACOMPILER_DFA_H

#include "../NFA/NFA.h"

class DFA_State {
public:

    int id;
    bool accepting;
    string acceptance_state;
    unordered_map<string, int> transitions;
   /** bool operator< (const DFA_State *other) const {
        return this->id < other->id;
    }**/
    DFA_State(int id);
};

class DFA {
public:

    vector<DFA_State> states ;

    DFA(vector<DFA_State> states);
    void transition_table(string path, set<string> chars);
};

class DFA_builder {
public:
    static DFA build_dfa(NFA);
    DFA minimize_dfa(vector<DFA_State>);
};

#endif //JAVACOMPILER_DFA_H
