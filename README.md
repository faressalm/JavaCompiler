# Phase 1: Lexical Analyzer Generator

## A description of the used data structures.

### Lexical Rule Parser

- A queue of pairs ,a pair consists of string and boolean → Queue is to hold postfix representation of RE 
→ pair `< character, isOperator>`

### NFA

```cpp
class State{
public:
    static int id_generator;
    unordered_map<string, vector<State*>> transitions;
    const int id;

    void addTransition(const string& label, State* next);
    unordered_set<State*> move(unordered_set<State*> &nodes, string label);
    unordered_set<State*> e_closure(unordered_set<State* > &nodes);
};
```

- static int id_generator: a generator to get a new id every time we make a new state.
- unordered_map<string, vector<State*>> transitions: a map holding for each state, its next states with the transition inputs leading to them.
- int id: whenever a new state is created in the NFA, it is given a id representing it.
- void addTransition(const string& label, State* next): method to add transition in transitions map.
- unordered_set<State*> move(unordered_set<State*> &nodes, string label): a method to move between states with a transition input.
- unordered_set<State*> e_closure(unordered_set<State* > &nodes): a method to get epsilon closure of a specific state.

```cpp
class NFA{
public:
    State* start;
    State* end;
    unordered_map<State*, pair<string, int>> acceptingStates;
    set<string> chars;
};
```

- State* start: the start state of NFA.
- State* end: the end state of NFA (not the accepting states).
- unordered_map<State*, pair<string, int>> acceptingStates: a map holding for each accepting state a pair of the name of the token that it accepts and the priority of it.
- set<string> chars: a set of possible transition inputs.

### DFA:

```cpp
class  DFA_State {
public:

    int id;
    bool accepting;
    pair<string,int> acceptance_state;
    unordered_map<string, int> transitions;
};
```

- int id: whenever a new state is created in the DFA, it is given a id representing it. That id is used in the minimization of DFA.
- bool accepting: a boolean indicating whether that state is an acceptance state
or not.
- pair<string, int> acceptance_state: a pair holding for each acceptance state the name of token that it accepts and the priority of it.
- unordered_map<string, int> transitions: a map holding for each state, its next indices of states with the transition inputs leading to them.

```cpp
class DFA {
public:
    vector<DFA_State> states ;
    int reject_state;

    void transition_table(string path, set<string> chars);
};
```

- vector<DFA_State> states: a vector of states that exist in our DFA.
- int reject_state: the index of the reject state in the vector of states.
- void transition_table(string path, set<string> chars): a method to get transition table.

---

## Explanation of all algorithms and techniques used

### **Shunting-Yard Algorithm for**  Lexical Rule Parser

- If the **input symbol is a letter**… append it directly to the output queue
- If the **input symbol is an operator**… if there exists an operator already on the top of the operator stack with higher or equal precedence than our current input symbol, remove
the operator from the top of the operator stack and append it to the output queue. Do this until the current input symbol has higher precedence than the symbol on the top of the operator stack, or the operator stack is empty.
- If the **input symbol is an operator AND there is a left parenthesis on top of the stack**… push the input symbol onto the stack on top of the left parenthesis.
- If the **input symbol is an “(”** … append it to the operator stack
- If the **input symbol is an “)”** … pop all operators from the operator stack and append them to the output queue until you find an “**(”**. Then, you can remove both of those parentheses and continue with the algorithm.
- EX: `a.(a+b)*.b`  → `aab+*.b.`

---

### Thompson’s Algorithm

- `NFA concatenate(NFA &a, NFA &b):` It concatenates two NFAs as the end of the first reaches the start of the second by an epsilon transition.
- `NFA Or(NFA &a, NFA &b):` It creates a new NFA by oring between the two NFA parameters. Two new states are created, one at the start and one at the end. That start goes with two epsilon transitions to each start of the NFAs sent. Their ends go to that end node also with
one epsilon transition for each NFA.
- `NFA kleene_closure(NFA &a):` It creates a new NFA representing the zero or more transition.
- `NFA positive_closure(NFA &a):` It creates a new NFA representing the one or more transition.
- `NFA hyphen(NFA &a, NFA &b):` It creates an NFA representing a range or input for the (a-z | A-Z). It creates the NFA to accept any input in that range.
- `NFA build(const vector<queue<pair<string, bool>>>& REs, const vector<pair<string, int>>& tokens):` Given a vector of REs, each one in a queue. Calling build_nfa to build each one of the NFA then combine them all using or function.
- `NFA build_nfa(queue<pair<string, bool>> re, const pair<string, int>& token):` given a queue of tokens in a postfix format, we use a stack to keep track of each NFA built then when we face any operator we pop its operands from the stack and push the result again.

### Subset Construction

- `unordered_set<State*> e_closure(unordered_set<State*> &nodes):` For any set of NFA states, this method returns a set containing their epsilon closure states.
- `DFA build_dfa(NFA):` It represents the main flow for building the DFA from an NFA represented by the NFAs start, adds the reject state to DFA, iterates over the newly formed states to get their epsilon closure, adds the newly formed state to the queue if it is not visited, and iterates over the transition of the current state and insert the transitions with the non-epsilon value.
- `DFA minimize_dfa(vector<DFA_State>, int):` It takes a vector of DFA_States and the reject_state index then it follows the Moore’s algorithm in minimization to get the equivalent states, then it combined all the common states in single partition for each set of state, after that it create new states from this partitions and returns the minimized states,
- `void transition_table(string path, set<string> chars):` This method makes a transition table of the minimized DFA in a csv file.

### Moore’s Algorithm

- Create a table of pairs (p, q) where p, q denotes some two states of our DFA. Initially, all the table cells are unmarked.
- Mark all the pairs (p, q) such that p belongs to acceptance state and q does not belong to the same acceptance state, or does not belong to acceptance state at all
- If (p, q) is unmarked and there exists a transition that move it to a marked pair of states then mark q and p
- repeat the above process until no new pairs are marked
- After completing the above process, p is equivalent to q if and only if (p, q) is unmarked. The equivalent states can be compressed to get the minimum number of states.

## The resultant stream of tokens for the example test program.

```
int
id
,
id
,
id
,
id
;
while
(
id
relop
num
)
{
id
assign
id
addop
num
;
}
```

## Any assumptions made and their justification.

1. Assume RE found first has a higher priority. keywords  and punctuations have the highest priority 
2. Each keyword must be  separated from others with spaces
3. There will be no expression(RD or RE) that's  contracted from other RDs tokens names before the contracted ones are defined 
4. The RD with a token name that's part of another RD token name will be defined first
5. When parsing input program file `(' ','\n','\t')` are used as a terminal character for current token 
6. we used `.` as a concatenation operator with boolean `isOperator = true` to be distinguished from the normal alphabet 
7. recover only from invalid program alphabet

# Phase 2: Parser Generator

## Description of the used data structures.

1. ****************************Parser Token Class****************************
    - it has a type which is one of the following `[Terminal, NonTerminal, Epsilon]`
    - name of this token
2. **Production Rule Class** 
    - NonTerminal name
    - List of rules which is a List of productions where each production is a list of tokens
    - boolean hasEpsilon
    - index for epsilon production if exists
3. Queue of string to save lexical generated tokens
4. Stack of parser tokens to save current tokens used in Top-Down stack algorithm   
5. Map of parser tokens as key and the value is a map of parser tokens and a pair of string and vector of parser tokens

## Explanation of all algorithms and techniques used

> For generating productions split input on `#` then for each NonTerminal assignment production split on `||`
> 

### 1. TopDown algorithm

- When the queue of lexical tokens is empty, get the next token until no more inputs
- compare this token with the top of the stack of parser tokens and apply algorithm as mentioned in the lecture

### 2. Computing the first set of any production

- First(α) is a set of terminal symbols that begin in strings derived from α.
- We have 3 rules for computing the first set:
    - For a production rule X → ∈, First(X) = { ∈ }
    - For any terminal symbol ‘a’, First(a) = { a }
    - For a production rule X → Y1Y2Y3,
        - If ∈ ∉ First(Y1), then First(X) = First(Y1)
        - If ∈ ∈ First(Y1), then First(X) = { First(Y1) – ∈ } ∪ First(Y2Y3)

### 3. Computing the follow set of any non-terminal

- Follow(α) is a set of terminal symbols that appear immediately to the right of α.
- We have 3 rules for computing the follow set:
    - For the start symbol S, place $- end of file marker- in Follow(S).
    - For any production rule A → αBβ,
        - If ∈ ∉ First(β), then Follow(B) = First(β)
        - If ∈ ∈ First(β), then Follow(B) = { First(β) – ∈ } ∪ Follow(A)
    - For any production rule A → αB, Follow(B) = Follow(A)

### 4. Construct parser table

- for each non terminal:
    - get its first set
    - get its follow set
    - add each production in the first set to the row except the epsilon if existed
    - add “sync” for each production in the follow set or add the production itself if there is an epsilon in the first set to the row
- add the non terminal and its row to the table

## The resultant stream of TopDown parser outputs for the example test program.

```cpp
METHOD_BODY → STATEMENT_LIST 
STATEMENT_LIST → STATEMENT STATEMENT_LIST_DASH 
STATEMENT → DECLARATION 
DECLARATION → PRIMITIVE_TYPE id ; 
PRIMITIVE_TYPE → int 
Match: int
Match: id
Match: ;
STATEMENT_LIST_DASH → STATEMENT STATEMENT_LIST_DASH 
STATEMENT → ASSIGNMENT 
ASSIGNMENT → id assign EXPRESSION ; 
Match: id
Match: assign
EXPRESSION → SIMPLE_EXPRESSION EXPRESSION_DASH 
SIMPLE_EXPRESSION → TERM SIMPLE_EXPRESSION_DASH 
TERM → FACTOR TERM_DASH 
FACTOR → num 
Match: num
TERM_DASH → \L 
SIMPLE_EXPRESSION_DASH → \L 
EXPRESSION_DASH → \L 
Match: ;
STATEMENT_LIST_DASH → STATEMENT STATEMENT_LIST_DASH 
STATEMENT → IF 
IF → if ( EXPRESSION ) { STATEMENT } else { STATEMENT } 
Match: if
Match: (
EXPRESSION → SIMPLE_EXPRESSION EXPRESSION_DASH 
SIMPLE_EXPRESSION → TERM SIMPLE_EXPRESSION_DASH 
TERM → FACTOR TERM_DASH 
FACTOR → id 
Match: id
TERM_DASH → \L 
SIMPLE_EXPRESSION_DASH → \L 
EXPRESSION_DASH → relop SIMPLE_EXPRESSION 
Match: relop
SIMPLE_EXPRESSION → TERM SIMPLE_EXPRESSION_DASH 
TERM → FACTOR TERM_DASH 
FACTOR → num 
Match: num
TERM_DASH → \L 
SIMPLE_EXPRESSION_DASH → \L 
Match: )
Match: {
STATEMENT → ASSIGNMENT 
ASSIGNMENT → id assign EXPRESSION ; 
Match: id
Match: assign
EXPRESSION → SIMPLE_EXPRESSION EXPRESSION_DASH 
SIMPLE_EXPRESSION → TERM SIMPLE_EXPRESSION_DASH 
TERM → FACTOR TERM_DASH 
FACTOR → num 
Match: num
TERM_DASH → \L 
SIMPLE_EXPRESSION_DASH → \L 
EXPRESSION_DASH → \L 
Match: ;
Match: }
Error: missing else, inserted
Error: missing {, inserted
Sync: discard STATEMENT
Error: missing }, inserted
STATEMENT_LIST_DASH → \L 
Match: $
```

## Any assumptions made and their justification

1. We change in the production rule files `=` → ‘assign’ to be compatible with the previous phase lexical rules 
2. Assume the Non-Terminals are separated by spaces in CFG File 
3. Assume at the end if no more lexical tokens and still there is Parser tokens the program will terminate since this case may happen when this is a parser token that has an empty entry in the parser table corresponding to the `$` Terminal token. 

---

## Bonus Part

### Left factoring:

- `vector<ProductionRule> leftFactor(vector<ProductionRule> productionRules, ProductionRule start):` It loops over all rules and call leftFactorOneRule() for each of them, then remove the old rule and add the new rules (repeat until there is no left factoring again).
- `vector<ProductionRule> leftFactorOneRule(ProductionRule productionRule):` It checks if that the rule has two or more alternatives with a common non-empty prefix by using maxMatchTokens() method and converts it into a new rules that don’t have left factoring (repeat until there is no left factoring in the same rule to handle nested left factoring) [e.g., (A → CB1 | … | CB5 | D1 | … | D5) to (A → CA’ | D1 | … | D5 and A’ → B1 | … | B5)].
- `int maxMatchTokens(vector<ParserToken> a, vector<ParserToken> b):` It returns the index of the first uncommon token between two productions.

### Eliminating left recursion:

The left-recursion may appear in a single step of the derivation (immediate left-recursion), or may appear in more than one step of the derivation, so we handle these two cases.

- `vector<ProductionRule> eliminateLR(vector<ProductionRule> productionRules, ProductionRule start):` It loops over all rules after substitute by all previous rules (to handle non-immediate left recursion) and call eliminateLROneRule() for each of them, then remove the old rule and add the new rules (repeat until there is no left recursion again).
- `vector<ProductionRule> eliminateLROneRule(ProductionRule productionRule):` It checks if that the rule has a left recursion or not by using isLeftRecursion() and if it has left recursion, it coonverts it into a new rules that don’t have left recursion by using getNonLRandLR() [e.g., (A → AB1 | … | AB5 | D1 | … | D5) to (A → D1A’ | … | D5A’ and A’ → B1A’ | … | B5A’ | epsilon)].
- `bool isLeftRecursion(ProductionRule productionRule):` It checks if the rule has a left recursion or not.
- `pair<vector<vector<ParserToken>>, vector<vector<ParserToken>>> getNonLRandLR(ProductionRule productionRule):` It splits the productions to a two sets one contains the productions with left recursion and another contains the productions without left recursion.
