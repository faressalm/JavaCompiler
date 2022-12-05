#include "DFA.h"

using namespace std;

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
