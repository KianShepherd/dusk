./clean.sh
flex -d src/tokens.l
bison --debug -Dparse.trace -v src/grammar.y

g++ --std=c++11 -DDEBUGGING -Wall src/dusk.cc src/parser.cc src/scanner.cc src/ast.cc src/expression.cc src/statement.cc src/function.cc -o dusk
