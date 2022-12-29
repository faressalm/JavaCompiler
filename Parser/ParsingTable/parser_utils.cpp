
#include "parser_utils.h"
#include "../production_rule.h"
ParserUtils::ParserUtils(vector<ProductionRule> &rules) {
  for(ProductionRule r : rules){
      ParserToken symbol = ParserToken( ParserToken::Type::NonTerminal, r.name);
      nonTerminalsList.push_back(symbol);
      this->grammar.insert({symbol, r});
  }
  ParserToken start_symbol = ParserToken(ParserToken::Type::NonTerminal , rules[0].name );
  ParserToken end_symbol = ParserToken(ParserToken::Type::Terminal,"$");
  for(auto it : this->grammar){
      run_first(it.first, it.second);
      this->follow.insert({it.first, {}});
  }
  this->follow.at(start_symbol).insert(end_symbol); // rule 1
  this->apply_follow_rule_two();
  this->apply_follow_rule_three();
  create_table();
}


unordered_set<ParserToken> ParserUtils::get_first(ParserToken &symbol) {
    if(symbol.type == ParserToken::Type::NonTerminal){
        return this->first.at(symbol);
    }
    unordered_set<ParserToken> first_set = {symbol};
    return first_set;
}
unordered_set<ParserToken> ParserUtils::get_follow(ParserToken &symbol) {
    if(symbol.type != ParserToken::Type::NonTerminal) {
        throw logic_error("Can't derive follow set for a not non_terminal symbol");
    }
    return this->follow.at(symbol);
}
vector<ParserToken> ParserUtils::get_first_production(ParserToken &non_terminal, ParserToken &first) {

    bool found = false;
    vector<ParserToken> ret ;
    ProductionRule prods = this->grammar.at(non_terminal);

    for(auto prod : prods.rules){
        unordered_set<ParserToken> curr_first = {eps_symbol};
        for(auto symbol : prod){
            const unordered_set<ParserToken> first_set = this->get_first(symbol);
            curr_first.insert(first_set.begin(), first_set.end());
            if(first_set.find(eps_symbol) == first_set.end() ){
                curr_first.erase(eps_symbol);
                break;
            }
        }
        if(curr_first.find(first) != curr_first.end()){
          ret = prod;
          if(!found){
              found = true ;
          }
          else{
              cout << "Error because " << first.name << " " << first.type << "\n";
              throw logic_error("2 production can't drive the same first symbol");
          }
        }
    }
    return  ret;
}

void ParserUtils::apply_follow_rule_two(){

    for(auto it : this->grammar ){
        for(auto prod : it.second.rules){
            unordered_set<ParserToken> curr_first;
            for(int i = prod.size() - 1 ; i >= 0 ; i-- ){
               if(prod[i].type == ParserToken::Type::NonTerminal){
                   this->follow.at(prod[i]).insert(curr_first.begin(), curr_first.end());
                   // update curr_first
                   unordered_set<ParserToken> first_set = this->first.at(prod[i]);
                   if(first_set.find(eps_symbol) == first_set.end()){
                       // reset
                       curr_first.clear();
                   }
                   else{
                       first_set.erase(eps_symbol);
                   }
                   curr_first.insert(first_set.begin(), first_set.end());
               }
               else if( prod[i].type == ParserToken::Type::Terminal) {
                   curr_first.clear();
                   curr_first.insert(prod[i]);
               }
            }
        }
    }
}

void ParserUtils::apply_follow_rule_three(){
    while (true){ // keep apply the rule until there is no change in the non-terminals follow sets
        bool change = false;
        int old_size, new_size;
        for(auto  it : this->grammar){
            const unordered_set<ParserToken> curr_follow = this->follow.at(it.first);
            for(auto prod : it.second.rules){
                for(int i = prod.size() - 1 ; i >= 0 && prod[i].type == ParserToken::Type::NonTerminal ; i-- ) {
                          unordered_set<ParserToken>& follow_set = this->follow.at(prod[i]);
                          old_size = follow_set.size();
                          follow_set.insert(curr_follow.begin(), curr_follow.end());
                          new_size = follow_set.size();
                          change |= (old_size != new_size);
                          unordered_set<ParserToken> first_set = this->first.at(prod[i]);
                          if(first_set.find(eps_symbol) == first_set.end()){
                              break;
                          }
                }

            }
        }
        if (!change) break;
    }
}
void ParserUtils::run_first(const ParserToken &non_terminal, const ProductionRule &prods) {
   if(this->first.find(non_terminal) != this->first.end()){
       return; // already computed
   }

  this->first.insert({non_terminal, {eps_symbol} });
  unordered_set<ParserToken>& curr_first = this->first.at(non_terminal) ;

  bool has_eps = false; // control possibility of adding eps to the first set
  for(auto prod : prods.rules){
      bool curr_eps = true; // check if eps should be added to the first set of this production
      for(auto symbol : prod){
          if(symbol.type == ParserToken::Type::Epsilon){
              // do nothing
          }
          else if (symbol.type == ParserToken::Type::Terminal){
              curr_first.insert(symbol);
              curr_eps = false;
          }
          else if (symbol.type == ParserToken::Type::NonTerminal){
              run_first(symbol, this->grammar.at(symbol));
              unordered_set<ParserToken> first_set = this->first.at(symbol);
              curr_first.insert(first_set.begin(), first_set.end());
              if(first_set.find(eps_symbol) == first_set.end()){
                  curr_eps = false;
              }
          }
          if(!curr_eps) break;
      }
      has_eps |= curr_eps;
  }
  if(!has_eps){
      curr_first.erase(eps_symbol);
  }
}
//unordered_map<ParserToken nonterminal, unordered_map<ParserToken terminal, pair<string type, vector<ParserToken> production>>>
void ParserUtils::create_table(){
    vector<ParserToken> epsProd = {eps_symbol};

    for(auto nonTerm: this->nonTerminalsList) {
        unordered_set<ParserToken> firstSet = get_first(nonTerm);
        unordered_set<ParserToken> followSet = get_follow(nonTerm);
        unordered_map<ParserToken, pair<string, vector<ParserToken>>> tableRow;
        bool hasEpsilon = false;

        for (auto itr : firstSet) {
            if (itr.type == ParserToken::Type::Epsilon) {
                hasEpsilon = true;
                continue;
            }
            tableRow.insert({ itr, make_pair("production", get_first_production(nonTerm, itr))});
        }

        for (auto itr : followSet) {
            if (hasEpsilon) {
                tableRow.insert({ itr, make_pair("production", epsProd)});
            }
            else {
                tableRow.insert({itr, {"sync", {}}});
            }
        }
        this->parsingTable.insert({nonTerm, tableRow});
    }
    //cout << "a" <<"\n";
}

pair<string, vector<ParserToken>> ParserUtils::get_entry(ParserToken& non_terminal, ParserToken& terminal) {
    unordered_map<ParserToken, pair<string, vector<ParserToken>>> row = this->parsingTable.at(non_terminal);
    if (row.find(terminal) == row.end()) { //empty entry
        return {"error",{}};
    }
    return row.at(terminal);
}

void ParserUtils::print_parsing_table(string path, unordered_set<string> terminals) {
    ofstream myFile;
    vector<string> terminalsVector;

    for (auto itr : terminals) {
        terminalsVector.push_back(itr);
    }
    terminalsVector.push_back("$");

    myFile.open(path + "1");
    myFile << "non-terminal/terminal";
    for (auto itr : terminalsVector) {
        if (itr == ",") {
            myFile << ",\",\"";
        }
        else if (itr == ";") {
            myFile << ",\";\"";
        }
        else {
            myFile << "," << (itr);
        }
    }
    myFile << "\n";

    for (auto itr : this->parsingTable) {
        myFile << itr.first.name << "," ;
        for (auto term :terminalsVector) {
            ParserToken temp1 = itr.first;
            ParserToken temp2 = ParserToken(ParserToken::Type::Terminal, term);
            auto entry = get_entry(temp1, temp2);
            myFile << entry.first << ",";
        }
        myFile << "\n";
    }

    myFile.close();
}

string ParserUtils::get_production_name(vector<ParserToken> vec) {
    string str = "";
    for (auto itr : vec) {
        str.append(itr.name);
        str.append(" ");
    }
    return str;
}