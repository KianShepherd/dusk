%code requires {
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>
#include <iostream>
#include "expression.hh"
#include "statement.hh"
#include "function.hh"
#include "ast.hh"

extern FILE* yyin;
extern int yylineno;
int yylex();  
void yyerror(AST&, const char*);
}
%output "src/parser.cc"
%defines "src/parser.h"

%parse-param { AST& ast }

%union
{
    std::string* str;
    Expression* expr;
    Statement* stat;
    std::vector<Statement*>* stats;
    std::vector<Function*>* funcs;
}

%token<str> NUMBER STRING DOUBLE IDENTIFIER TRUE FALSE NULLTOK
%token<str> DIVIDE TIMES PLUS MINUS NOT EQUAL EQUALEQUAL BANGEQUAL LESSEQUAL MOREEQUAL LESSTHAN MORETHAN OR AND
%token<str> SEMICOLON LBRACE RBRACE LPAREN RPAREN COLON COMMA ARROW
%token<str> INT VOID FLOAT BOOL FUNCTION IF ELSE FOR WHILE RETURN LET MUTABLE
%token<str> LEXERROR;

%type<expr> exp
%type<stat> statement statementblock;
%type<stats> statements;
%type<str> functions function;

%left OR AND
%left EQUALEQUAL BANGEQUAL MORETHAN LESSTHAN MOREEQUAL LESSEQUAL
%left PLUS MINUS
%left TIMES DIVIDE
%left UNARY

%destructor { delete $$; } NUMBER STRING DOUBLE IDENTIFIER TRUE FALSE NULLTOK
%destructor { delete $$; } PLUS DIVIDE TIMES MINUS NOT EQUAL EQUALEQUAL BANGEQUAL LESSEQUAL MOREEQUAL LESSTHAN MORETHAN OR AND
%destructor { delete $$; } SEMICOLON LBRACE RBRACE LPAREN RPAREN COLON COMMA ARROW
%destructor { delete $$; } INT VOID FLOAT BOOL FUNCTION IF ELSE FOR WHILE RETURN LET MUTABLE
%%
functions: %empty
    | functions function
    {
        $$ = new std::string("a");
    }
    ;

function: FUNCTION IDENTIFIER LPAREN RPAREN statementblock
    {
        Function* f = new Function(std::string(*($2)), $5);
        ast.push_function(f);
        $$ = new std::string("a");
    }
    ;

statementblock: LBRACE statements RBRACE
    {
        $$ = new StatementBlock(*($2));
    }
    ;

statements:%empty
    {
        $$ = new std::vector<Statement*>();
    }
    | statements statement
    {
        if (!$1) {
            $$ = new std::vector<Statement*>();
        } else {
            $$ = $1;
        }
        $$->push_back($2);
    }
    | statements statementblock
    {
        if (!$1) {
            $$ = new std::vector<Statement*>();
        } else {
            $$ = $1;
        }
        $$->push_back($2);
    }
    ;

statement: exp SEMICOLON
    {
        $$ = new ExpressionStatement($1);
    }
    ;

exp: NUMBER
    {
        $$ = new ExpressionAtomic(std::stoll(*($1)));
    }
    | STRING
    {
        $$ = new ExpressionAtomic(*($1), false);
    }
    | DOUBLE
    {
        $$ = new ExpressionAtomic(std::stod(*($1)));
    }
    | TRUE
    {
        $$ = new ExpressionAtomic(true);
    }
    | FALSE
    {
        $$ = new ExpressionAtomic(false);
    }
    | NULLTOK
    {
        $$ = new ExpressionAtomic();
    }
    | IDENTIFIER
    {
        $$ = new ExpressionAtomic(*($1), true);
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
    | exp DIVIDE exp
    {
        $$ = new BinaryExpression($1, $3, std::string("/"));
    }
    | exp MORETHAN exp
    {
        $$ = new BinaryExpression($1, $3, std::string(">"));
    }
    | exp LESSTHAN exp
    {
        $$ = new BinaryExpression($1, $3, std::string("<"));
    }
    | exp MOREEQUAL exp
    {
        $$ = new BinaryExpression($1, $3, std::string(">="));
    }
    | exp LESSEQUAL exp
    {
        $$ = new BinaryExpression($1, $3, std::string("<="));
    }
    | exp EQUALEQUAL exp
    {
        $$ = new BinaryExpression($1, $3, std::string("=="));
    }
    | exp BANGEQUAL exp
    {
        $$ = new BinaryExpression($1, $3, std::string("!="));
    }
    | exp AND exp
    {
        $$ = new BinaryExpression($1, $3, std::string("and"));
    }
    | exp OR exp
    {
        $$ = new BinaryExpression($1, $3, std::string("or"));
    }
    ;

exp: NOT exp %prec UNARY
    {
        $$ = new UnaryExpression($2, std::string("!"));
    }
    | MINUS exp %prec UNARY
    {
        $$ = new UnaryExpression($2, std::string("-"));
    }
    | LPAREN exp RPAREN
    {
        $$ = new UnaryExpression($2, std::string("()"));
    }
    ;

err: LEXERROR
     {
        ast.push_err($1);
     }
     ;

