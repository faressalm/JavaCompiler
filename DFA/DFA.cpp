#include "DFA.h"
#include "algorithm"

using namespace std;


DFA_State::DFA_State(int id){
    this->id = id;
    this->accepting = false;
}
void check_acceptance(DFA_State& state , unordered_set<State*>elem, NFA& nfa );
bool same_states(DFA_State state1, DFA_State state2, vector<vector<int>> mark_table, vector<DFA_State>& states);
DFA::DFA(vector<DFA_State> states) {
  this->states = states;
}

void DFA::transition_table(string path, set<string> chars) {
    ofstream myFile;
    myFile.open(path);
    set<string>::iterator itr;
    myFile << "State/Input";
    myFile << "," << "Accepting?";
    int counter = 0;
    for (itr = chars.begin(); itr != chars.end(); itr++) {
        counter++;
        if (*itr == ",")
            myFile << ",\",\"";
        else
            myFile << "," << (*itr);
    }

    myFile << "\n";
    for (auto state : this->states) {
        myFile << state.id;
        myFile << "," << state.accepting;
        for (itr = chars.begin(); itr != chars.end(); itr++)
            myFile << "," << state.transitions.find(*itr)->second;

        myFile << "\n";
    }

    myFile.close();
}

DFA DFA_builder::build_dfa(NFA combinedNFA) {
    unordered_map< unordered_set<State*> , int  > visited;
    queue< unordered_set<State*> > q ; // unvisited
    unordered_set<State*> nodes;
    State* helper = new State();
    nodes.insert(combinedNFA.start);
    nodes = helper->e_closure(nodes);
    q.push(nodes);

    vector<DFA_State> states ;
    int id = 0 ;
    visited.insert({ nodes , id });
    states.push_back(DFA_State(id++));
    while ( !q.empty()){
        unordered_set<State*> elem = q.front();
        q.pop();
        int idx = visited.at(elem);
        check_acceptance(states[idx] , elem, combinedNFA );
        for(string label : combinedNFA.chars){
            unordered_set<State*> next = helper->move(elem , label); // could be empty state {}
            next = helper->e_closure(next);
            auto itr = visited.find(next);
            if( itr == visited.end()){
                itr = visited.insert({next , id}).first ;
                q.push(next);
                states.push_back(DFA_State(id++));
            }
            states[idx].transitions[label] = itr->second;
        }
    }
    // TODO: Apply minimization @Monem, you have vector of states
    DFA dfa(states);
    return dfa;
}

void check_acceptance(DFA_State& state , unordered_set<State*>elem, NFA& nfa ){
    int priority = INT_MIN;
    unordered_set<State*>::iterator itr = elem.begin();
    while(itr != elem.end()){
        auto check =nfa.acceptingStates.find( (*itr) );
        if( check != nfa.acceptingStates.end() ){
             if(check->second.second > priority ){
                 state.accepting = true;
                 state.acceptance_state = check->second.first;
                 priority = check->second.second;
             }
        }
        itr++;
    }
}

DFA DFA_builder::minimize_dfa(vector<DFA_State> dfa_States) {
    int number_of_states = dfa_States.size();
    vector< DFA_State > dfa_copy = dfa_States;
    //sort(dfa_copy.begin(), dfa_copy.end());
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
                             temp1.acceptance_state != temp2.acceptance_state) {
                        mark_table[row][col] = 1;
                    }
                    else {
                        if (!same_states(temp1, temp2, mark_table, dfa_States)) {
                            mark_table[row][col] = 1;
                        }
                    }
                } else {
                    break;
                }
            }
        }
    }

    // to be implemented
    vector<DFA_State> d;
    DFA dfa(d);
    return dfa;
}

bool same_states(DFA_State state1, DFA_State state2, vector<vector<int>> mark_table, vector<DFA_State>& states) {
    if (state1.transitions.size() != state2.transitions.size()) return false;
    auto m1 = state1.transitions;
    auto m2 = state2.transitions;
    for (auto i: m1) {
        if (m2.find(i.first) == m2.end()) return false;
        DFA_State transition1 = states[m1[i.first]];
        DFA_State transition2 = states[m2[i.first]];
        int row = max(transition1.id, transition2.id);
        int col = min(transition1.id, transition2.id);
        if (mark_table[row][col] == 1) return false;
        return true;
    }
}