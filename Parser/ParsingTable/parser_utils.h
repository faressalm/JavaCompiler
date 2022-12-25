

#ifndef PARSER_UTILS_H
#define PARSER_UTILS_H

#include "../production_rule.h"
const ParserToken eps_symbol = ParserToken(ParserToken::Type::Epsilon , "\\L");

class ParserUtils{
public:
    ParserUtils(vector<ProductionRule>& rules);
    /**
     * Returns a first set of any symbol
     * @param symbol
     * @return first_set
     */
    unordered_set<ParserToken> get_first(ParserToken& symbol);
    /**
     * Returns the production that derives a symbol first in the first symbol
     * @param non_terminal non_terminal of interest
     * @param first first symbol that is in the first_set of the above non_terminal
     * @return production
     */
    vector<ParserToken> get_first_production(ParserToken& non_terminal, ParserToken& first);

    unordered_set<ParserToken> get_follow(ParserToken& symbol);
    unordered_map<ParserToken, unordered_set<ParserToken>> first ;
private:
    unordered_map<ParserToken , ProductionRule > grammar;

    unordered_map<ParserToken, unordered_set<ParserToken>> follow ;
    void run_first(const ParserToken& non_terminal,const ProductionRule& prods);
    void apply_follow_rule_two();
    void apply_follow_rule_three();
};
#endif //PARSER_UTILS_H
