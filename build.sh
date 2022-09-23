flex src/tokens.l
bison src/grammar.y

g++ --std=c++11 -Wall src/dusk.cc src/parser.cc src/scanner.cc src/ast.cc src/expression.cc -o dusk
