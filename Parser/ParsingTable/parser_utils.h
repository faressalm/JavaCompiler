

#ifndef PARSER_UTILS_H
#define PARSER_UTILS_H

#include "../production_rule.h"
const ParserToken eps_symbol = ParserToken(ParserToken::Type::Epsilon , "\\L");

class ParserUtils{
public:
    using table = unordered_map<ParserToken, unordered_map<ParserToken, pair<string, vector<ParserToken>>>>;
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

    void create_table();

    pair<string, vector<ParserToken>> get_entry(ParserToken& non_terminal, ParserToken& terminal);

    void print_parsing_table(string path, unordered_set<string> terminals);
    string get_production_name(vector<ParserToken> vec);
private:
    unordered_map<ParserToken , ProductionRule > grammar;
    unordered_map<ParserToken, unordered_set<ParserToken>> first ;
    unordered_map<ParserToken, unordered_set<ParserToken>> follow ;
    vector<ParserToken> nonTerminalsList;
    table parsingTable;
    void run_first(const ParserToken& non_terminal,const ProductionRule& prods);
    void apply_follow_rule_two();
    void apply_follow_rule_three();
};
#endif //PARSER_UTILS_H
