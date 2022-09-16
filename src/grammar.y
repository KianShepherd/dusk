%code requires {
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>
#include <iostream>
#include "expression.hh"

using Vec = std::vector<Expression*>;


extern FILE* yyin;
extern int yylineno;
int yylex();  
void yyerror(Vec&, const char*);
}
%output "src/parser.cc"
%defines "src/parser.h"

%parse-param { Vec& exprs }

%union
{
    std::string* str;
    Expression* expr;
}

%token<str> NUMBER STRING DOUBLE IDENTIFIER TRUE FALSE NULLTOK
%token<str> DIVIDE TIMES PLUS MINUS NOT EQUAL EQUALEQUAL BANGEQUAL LESSEQUAL MOREEQUAL LESSTHAN MORETHAN OR AND
%token<str> SEMICOLON LBRACE RBRACE LPAREN RPAREN COLON COMMA ARROW
%token<str> INT VOID FLOAT BOOL FUNCTION IF ELSE FOR WHILE RETURN LET MUTABLE

%type<expr> exp

%left PLUS MINUS
%left TIMES DIVIDE

%destructor { delete $$; } NUMBER STRING DOUBLE IDENTIFIER TRUE FALSE NULLTOK
%destructor { delete $$; } PLUS DIVIDE TIMES MINUS NOT EQUAL EQUALEQUAL BANGEQUAL LESSEQUAL MOREEQUAL LESSTHAN MORETHAN OR AND
%destructor { delete $$; } SEMICOLON LBRACE RBRACE LPAREN RPAREN COLON COMMA ARROW
%destructor { delete $$; } INT VOID FLOAT BOOL FUNCTION IF ELSE FOR WHILE RETURN LET MUTABLE
%%

statements: %empty
    | statements statement
    ;

exp: NUMBER
    {
        $$ = new ExpressionAtomic(std::stoll(*($1)));
    }
    | STRING
    {
        $$ = new ExpressionAtomic(*($1));
    }
    | DOUBLE
    {
        $$ = new ExpressionAtomic(std::stod(*($1)));
    }
    ;

exp: exp PLUS exp
    {
        $$ = new BinaryExpression($1, $3, std::string("+"));
    }
    | exp MINUS exp
    {
        $$ = new BinaryExpression($1, $3, std::string("-"));
    }
    | exp TIMES exp
    {
        $$ = new BinaryExpression($1, $3, std::string("*"));
    }
    |
    exp DIVIDE exp
    {
        $$ = new BinaryExpression($1, $3, std::string("/"));
    }
    ;

statement: exp SEMICOLON
    {
        exprs.push_back($1);
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
        std::cout << "-----" << std::endl;
    }
    return rc;
}
