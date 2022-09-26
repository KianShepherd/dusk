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
    Function* func;
    std::vector<Statement*>* stats;
    std::vector<Function*>* funcs;
}

%token<str> NUMBER STRING DOUBLE IDENTIFIER TRUE FALSE NULLTOK
%token DIVIDE TIMES PLUS MINUS NOT EQUAL EQUALEQUAL BANGEQUAL LESSEQUAL MOREEQUAL LESSTHAN MORETHAN OR AND
%token SEMICOLON LBRACE RBRACE LPAREN RPAREN COLON COMMA ARROW
%token INT VOID FLOAT BOOL STR FUNCTION IF ELSE FOR WHILE RETURN LET MUTABLE
%token<str> LEXERROR;

%type<expr>  exp;
%type<stat>  statement statementblock;
%type<stats> statements;
%type<func>  function;
%type<funcs> functions;

%left OR AND
%left EQUALEQUAL BANGEQUAL MORETHAN LESSTHAN MOREEQUAL LESSEQUAL
%left PLUS MINUS
%left TIMES DIVIDE
%left UNARY

%destructor { delete $$; } NUMBER STRING DOUBLE IDENTIFIER TRUE FALSE NULLTOK

%%
functions: %empty
    {
        $$ = new std::vector<Function*>();
    }
    | functions function
    {
        $$->push_back($2);
    }
    ;

function: FUNCTION IDENTIFIER LPAREN RPAREN statementblock
    {
        Function* f = new Function(std::string(*($2)), $5, t_null);
        ast.push_function(f);
        $$ = f;
    }
    | FUNCTION IDENTIFIER LPAREN RPAREN ARROW BOOL statementblock
    {
        Function* f = new Function(std::string(*($2)), $7, t_bool);
        ast.push_function(f);
        $$ = f;
    }
    | FUNCTION IDENTIFIER LPAREN RPAREN ARROW INT statementblock
    {
        Function* f = new Function(std::string(*($2)), $7, t_number);
        ast.push_function(f);
        $$ = f;
    }
    | FUNCTION IDENTIFIER LPAREN RPAREN ARROW FLOAT statementblock
    {
        Function* f = new Function(std::string(*($2)), $7, t_float);
        ast.push_function(f);
        $$ = f;
    }
    | FUNCTION IDENTIFIER LPAREN RPAREN ARROW STR statementblock
    {
        Function* f = new Function(std::string(*($2)), $7, t_string);
        ast.push_function(f);
        $$ = f;
    }
    ;

statementblock: LBRACE statements RBRACE
    {
        $$ = new StatementBlock(*($2));
    }
    ;

statements: %empty
    {
        $$ = new std::vector<Statement*>();
    }
    | statements statement
    {
        $$->push_back($2);
    }
    | statements statementblock
    {
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
        free($1);
    }
    | STRING
    {
        $$ = new ExpressionAtomic(std::string(*($1)), false);
        free($1);
    }
    | DOUBLE
    {
        $$ = new ExpressionAtomic(std::stod(*($1)));
        free($1);
    }
    | TRUE
    {
        $$ = new ExpressionAtomic(true);
        free($1);
    }
    | FALSE
    {
        $$ = new ExpressionAtomic(false);
        free($1);
    }
    | NULLTOK
    {
        $$ = new ExpressionAtomic();
        free($1);
    }
    | IDENTIFIER
    {
        $$ = new ExpressionAtomic(std::string(*($1)), true);
        free($1);
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

