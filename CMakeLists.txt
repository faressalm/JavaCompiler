cmake_minimum_required(VERSION 3.23)
project(JavaCompiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(JavaCompiler main.cpp LexicalAnalyzer/NFA/NFA.h LexicalAnalyzer/NFA/NFA.cpp LexicalAnalyzer/DFA/DFA.h LexicalAnalyzer/DFA/DFA.cpp LexicalAnalyzer/LexicalRulesParser/lexical_rules_generator.h LexicalAnalyzer/LexicalRulesParser/lexical_rules_generator.cpp LexicalAnalyzer/lexical_analyzer.h LexicalAnalyzer/lexical_analyzer.cpp LexicalAnalyzer/DFA/DFA.cpp LexicalAnalyzer/DFA/DFA.h Tests/lexicalTests.cpp)
