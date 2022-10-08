./clean.sh
flex -d src/tokens.l
bison --debug -Dparse.trace -v src/grammar.y

g++  -Wall -I ./include src/dusk.cc src/parser.cc src/scanner.cc src/ast.cc src/expression.cc src/statement.cc src/function.cc `llvm-config --cxxflags --ldflags --system-libs --libs all` -o dusk
