lex -o src/lex.yy.c src/tokens.l
yacc -d src/grammar.y -o src/y.tab.cc

gcc -c src/lex.yy.c -o src/lex.yy.o
g++ src/lex.yy.o src/y.tab.cc -o dusk

