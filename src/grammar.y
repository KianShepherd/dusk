%code requires {
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>
#include <iostream>
#include "expression.hh"

using Vec = std::vector<Expression*>;

#define YYSTYPE std::string*

extern FILE* yyin;
extern int yylineno;
int yylex();  
void yyerror(Vec&, const char*);

}
%output "src/parser.cc"
%defines "src/parser.h"

%parse-param { Vec& exprs }

%token NUMBER STRING DOUBLE IDENTIFIER TRUE FALSE NULLTOK
%token DIVIDE TIMES PLUS MINUS NOT EQUAL EQUALEQUAL BANGEQUAL LESSEQUAL MOREEQUAL LESSTHAN MORETHAN OR AND
%token SEMICOLON LBRACE RBRACE LPAREN RPAREN COLON COMMA ARROW
%token INT VOID FLOAT BOOL FUNCTION IF ELSE FOR WHILE RETURN LET MUTABLE
%left PLUS MINUS
%left MULT DIV


%destructor { delete $$; } STRING DOUBLE IDENTIFIER TRUE FALSE NULLTOK
%destructor { delete $$; } DIVIDE TIMES MINUS NOT EQUAL EQUALEQUAL BANGEQUAL LESSEQUAL MOREEQUAL LESSTHAN MORETHAN OR AND
%destructor { delete $$; } SEMICOLON LBRACE RBRACE LPAREN RPAREN COLON COMMA ARROW
%destructor { delete $$; } INT VOID FLOAT BOOL FUNCTION IF ELSE FOR WHILE RETURN LET MUTABLE
%%

statements: %empty
    | statements statement
    ;

exp: NUMBER
    {
        exprs.push_back(new ExpressionAtomic(std::stoll(*$1)));
    }
    | STRING
    {

    }
    | DOUBLE
    {

    }
    ;

exp: exp PLUS exp
    {

    }
    | exp MINUS exp
    {

    }
    | exp MULT exp
    {

    }
    |
    exp DIV exp
    {

    }
    ;

statement: exp SEMICOLON
    {
        
    }
    ;
%%

void yyerror(Vec& expr, const char* msg) {
    std::cerr << msg << " at line " << yylineno << '\n';
}

int main(int argc, char** argv) {
    Vec expressions = Vec();
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            std::cerr << "Unable to open " << argv[1] << ": " << strerror(errno) << '\n';
            return 1;
        }
    } else {
        yyin = stdin;
    }
    int rc = yyparse(expressions);
    for (int i = 0; i < expressions.size(); i++) {
        expressions[i]->debug();
    }
    return rc;
}
