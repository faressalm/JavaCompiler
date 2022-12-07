#include "DFA.h"
#include "algorithm"

using namespace std;

void tokenize(string& s, DFA& dfa ){
    int curr = 0 , lastAcc = 0 ;
    if(dfa.states.size() == 0 ) return ;
    DFA_State pos = dfa.states[0];
    string label = "";
    string token = "";
    int priority = INT_MIN;
    //DFA_State last_acc_state = dfa.states[0];
    while(curr != (int) s.size()){
        label = "";
        token = "";
        priority = INT_MIN;
        label.push_back(s[curr]);
        if(pos.accepting && priority <= pos.acceptance_state.second){ // start might be an accepting state also
            lastAcc = curr;
            token = pos.acceptance_state.first;
            priority = pos.acceptance_state.second;
        }
        while(pos.id != dfa.reject_state  || pos.transitions.find(label) != pos.transitions.end()  ){
            pos = dfa.states[pos.transitions[label]];
            if(pos.accepting && priority <= pos.acceptance_state.second){
                lastAcc = curr;
                token = pos.acceptance_state.first;
                priority = pos.acceptance_state.second;
            }
            curr++;
            label.pop_back();
            label.push_back(s[curr]);
        }
        if(token != "") {
            cout << token << "\n";
        }
        pos = dfa.states[0];
        curr = lastAcc + 1;
    }
}
DFA_State::DFA_State(int id){
    this->id = id;
    this->accepting = false;
}
void check_acceptance(DFA_State& state , unordered_set<State*>elem, NFA& nfa );
bool same_states(DFA_State state1, DFA_State state2, vector<vector<int>> mark_table, vector<DFA_State>& states);
int find_in_partitions(int state_index, vector<set<int>> partitions);
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
    unordered_set<State*> empty_set;
    dfa.reject_state = visited.at(empty_set);
    vector<DFA_State> minimized_states =  minimize_dfa(states);
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
                 state.acceptance_state = check->second;
                 priority = check->second.second;
             }
        }
        itr++;
    }
}

vector<DFA_State> DFA_builder::minimize_dfa(vector<DFA_State> dfa_States) {
    int number_of_states = dfa_States.size();
    vector<vector<int>> mark_table(number_of_states, vector<int> (number_of_states, -1));
    // initialize mark table
    for (int i = 0; i <= number_of_states; ++i) {
        for (int row = 0; row < number_of_states; ++row) {
            for (int col = 0; col < number_of_states; ++col) {
                if (row > col) {
                    if (mark_table[row][col] == 1) continue;
                    DFA_State temp1 = dfa_States[row];
                    DFA_State temp2 = dfa_States[col];
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
    vector<set<int>> combinedStates;
    for (int row = 0; row < number_of_states; ++row) {
        for (int col = 0; col < number_of_states; ++col) {
            if (row > col) {
                if (mark_table[row][col] == -1) {
                    set<int> temp;
                    temp.insert(col);
                    temp.insert(row);
                    combinedStates.push_back(temp);
                }
            }
            else{
                break;
            }
        }
    }

    for (int i = 0; i < combinedStates.size(); ++i) {
        if (!combinedStates[i].empty()){
            set<int> tempSet1 = combinedStates[i];
            for (int j = i + 1; j < combinedStates.size(); ++j) {
                set<int> tempSet2 = combinedStates[j];
                set<int> intersect;
                set_intersection(tempSet1.begin(), tempSet1.end(), tempSet2.begin(), tempSet2.end(),
                                 inserter(intersect, intersect.begin()));
                if (!intersect.empty()){
                    tempSet1.insert(tempSet2.begin(), tempSet2.end());
                    set<int> empty_set;
                    combinedStates[j] = empty_set;
                    combinedStates[i] = tempSet1;
                }
            }
        }
    }

    vector<set<int>> final_partitions;
    for (int i = 0; i < combinedStates.size(); ++i) {
        set<int> temp_set = combinedStates[i];
        if (!temp_set.empty()) {
            final_partitions.push_back(temp_set);
        }
    }

    for (int i = 0; i < dfa_States.size(); ++i) {
        bool in_combined_state = false;
        int state_id = dfa_States[i].id;
        for (int j = 0; j < final_partitions.size(); ++j) {
            set<int> temp_set = final_partitions[j];
            if (temp_set.find(state_id) != temp_set.end()) {
                in_combined_state = true;
                break;
            }
        }
        if (!in_combined_state) {
            set<int> single_state_set;
            single_state_set.insert(state_id);
            final_partitions.push_back(single_state_set);
        }
    }
    vector<DFA_State> minimized_dfa;
    for (int i = 0; i < final_partitions.size(); ++i) {
        DFA_State myState = DFA_State(i);
        set<int> partition = final_partitions[i];
        DFA_State originalState = dfa_States[*partition.begin()];
        unordered_map<string, int> myMap;
        for (auto j : originalState.transitions) {
            myMap.insert({j.first, find_in_partitions(j.second, final_partitions)});
        }
        myState.transitions = myMap;
        myState.accepting = originalState.accepting;
        myState.acceptance_state = originalState.acceptance_state;
        minimized_dfa.push_back(myState);
    }
    return minimized_dfa;
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
    }
    return true;
}

int find_in_partitions(int state_index, vector<set<int>> partitions) {
    for (int i = 0; i < partitions.size(); ++i) {
        set<int> tempSet = partitions[i];
        if (tempSet.find(state_index) != tempSet.end()) {
            return i;
        }
    }
}