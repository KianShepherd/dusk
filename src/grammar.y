%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


extern "C" {

    int yyparse();

    int yylex(void);  

    int yywrap() {
            return 1;
    }

    void yyerror(const char* str) {
	    fprintf(stderr,"error: %s\n",str);
    }
}


extern "C" FILE* yyin;


int main(int argc, char** argv) {
    if (argc > 1) {
        FILE* fp = fopen(argv[1], "r");
        yyin = fp;
    }
    yyparse();
}

%}
%union 
{
        int number;
        double fl;
        char* string;
}

%token<number> NUMBER
%type<number> exp num;
%token <string> STRING
%type<string> str;
%token DIV MULT PLUS MINUS SEMICOLON
%left PLUS MINUS
%left MULT DIV

%%

statements: 
    |
    statements statement
    ;

str: STRING
    {
        $$ = $1;
    }
    ;

num: NUMBER
    {
        $$ = $1;
    }
    ;

exp: num
    {
        $$ = $1;
    }
    |
    exp PLUS exp
    {
        printf("expr %d + %d\n", $1, $3);
        $$ = $1 + $3;
    }
    |
    exp MINUS exp
    {
        printf("expr %d - %d\n", $1, $3);
        $$ = $1 - $3;
    }
    |
    exp MULT exp
    {
        printf("expr %d * %d\n", $1, $3);
        $$ = $1 * $3;
    }
    |
    exp DIV exp
    {
        printf("expr %d / %d\n", $1, $3);
        $$ = $1 / $3;
    }
    |
    str
    {
        printf("String %s\n", $1);
    }
    ;

statement: exp SEMICOLON
    {
        printf("statement\n");
    }
    ;

