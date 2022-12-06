#include "DFA.h"
#include "algorithm"

using namespace std;

bool same_states(DFA_State state1, DFA_State state2, vector<vector<int>> mark_table);
DFA::DFA() {
    this->start = new State();
}

// Not completed yet
DFA DFA_builder::build_dfa(NFA combinedNFA) {
    unordered_set<string> visited;
    unordered_set<State*> nfaStart;
    nfaStart.insert(combinedNFA.start);
    unordered_set<State*, MyHashFunction> epsClosureStart = combinedNFA.start->e_closure(nfaStart);
    DFA dfa;
    queue<unordered_set<State*, MyHashFunction>> unmarked;
    unmarked.push(epsClosureStart);
    unordered_map<string, unordered_set<State*, MyHashFunction>> dfaMap;
    vector<string> alphabet = combinedNFA.alphabet;     // We want to add alphabet to NFA class
    while (!unmarked.empty()) {
        unordered_set<State*, MyHashFunction> stateClosure = unmarked.front();
        unmarked.pop();
        string key;
        for (auto itr : stateClosure) {
            key.append(reinterpret_cast<const char *>(itr->id));
        }

        visited.insert(key);
        dfaMap.insert({key, stateClosure});
        for (auto cur_in = alphabet.begin(); cur_in != alphabet.end(); cur_in++) {
            unordered_set<State*, MyHashFunction> newState = dfa.start->e_closure(dfa.start->move(stateClosure, cur_in));   // Need to be edited
            key = "";
            for (auto itr : newState) {
                key.append(reinterpret_cast<const char *>(itr->id));
            }

            if (visited.find(key) == visited.end()) {
                unmarked.push(newState);
            }

            // Add the transition from stateClosure to newState at cur_in
        }
    }

    return dfa;
}

DFA DFA_builder::minimize_dfa(vector<DFA_State> dfa_States) {
    int number_of_states = dfa_States.size();
    vector< DFA_State > dfa_copy = dfa_States;
    sort(dfa_copy.begin(), dfa_copy.end());
    vector<vector<int>> mark_table(number_of_states, vector<int> (number_of_states, -1));
    // initialize mark table
    for (int i = 0; i <= number_of_states; ++i) {
        for (int row = 0; row < number_of_states; ++row) {
            for (int col = 0; col < number_of_states; ++col) {
                if (row > col) {
                    if (mark_table[row][col] == 1) continue;
                    DFA_State temp1 = dfa_copy[row];
                    DFA_State temp2 = dfa_copy[col];
                    if (temp1.accepting != temp2.accepting) { // accepting state with non-accepting state
                        mark_table[row][col] = 1;
                    }
                    // two different accepting states
                    else if (temp1.accepting && temp2.accepting &&
                             temp1.acceptance_state.first != temp2.acceptance_state.first) {
                        mark_table[row][col] = 1;
                    }
                    else {
                        if (!same_states(temp1, temp2, mark_table)) {
                            mark_table[row][col] = 1;
                        }
                    }
                } else {
                    break;
                }
            }
        }
    }

    DFA dfa;
    return dfa;
}

bool same_states(DFA_State state1, DFA_State state2, vector<vector<int>> mark_table) {
    if (state1.transitions.size() != state2.transitions.size()) return false;
    auto m1 = state1.transitions;
    auto m2 = state2.transitions;
    for (auto i: m1) {
        if (m2.find(i.first) == m2.end()) return false;
        DFA_State *transition1 = m1[i.first];
        DFA_State *transition2 = m2[i.first];
        int row = max(transition1->id, transition2->id);
        int col = min(transition1->id, transition2->id);
        if (mark_table[row][col] == 1) return false;
        return true;
    }
}