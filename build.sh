lex -o src/lex.yy.c src/grammar.l

gcc src/lex.yy.c -o dusk -lfl
