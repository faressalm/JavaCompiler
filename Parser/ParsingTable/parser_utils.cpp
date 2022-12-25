
#include "parser_utils.h"
#include "../production_rule.h"
const ParserToken eps= ParserToken(ParserToken::Type::Epsilon , "\\L");
ParserUtils::ParserUtils(vector<ProductionRule> &rules) {
  for(ProductionRule r : rules){
      ParserToken symbol = ParserToken( ParserToken::Type::NonTerminal, r.name)  ;
      this->grammar.insert({symbol, r});
  }
  for(auto it : this->grammar){
      run_first(it.first, it.second);
  }

}


unordered_set<ParserToken> ParserUtils::get_first(ParserToken &symbol) {
    if(symbol.type == ParserToken::Type::NonTerminal){
        return this->first.at(symbol);
    }
    unordered_set<ParserToken> first_set = {symbol};
    return first_set;
}
vector<ParserToken> ParserUtils::get_first_production(ParserToken &non_terminal, ParserToken &first) {
    bool found = false;
    vector<ParserToken> ret ;
    ProductionRule prods = this->grammar.at(non_terminal);
    for(auto prod : prods.rules){
        unordered_set<ParserToken> curr_first = {eps};
        for(auto symbol : prod){
            unordered_set<ParserToken> first_set = this->get_first(symbol);
            curr_first.insert(first_set.begin(), first_set.end());
            if(first_set.find(eps) != first_set.end()){
                curr_first.erase(eps);
                break;
            }
        }
        if(curr_first.find(first) != curr_first.end() && !found){
          found = true;
          ret = prod;
        }else{
            throw logic_error("2 production can't drive the same first symbol");
        }
    }
    return  ret;
}
void ParserUtils::run_first(const ParserToken &non_terminal, const ProductionRule &prods) {
   if(this->first.find(non_terminal) != this->first.end()){
       return; // already computed
   }

  this->first.insert({non_terminal, {eps} });
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
              if(first_set.find(eps) == first_set.end()){
                  curr_eps = false;
              }
          }
          if(!curr_eps) break;
      }
      has_eps |= curr_eps;
  }
  if(!has_eps){
      curr_first.erase(eps);
  }
}