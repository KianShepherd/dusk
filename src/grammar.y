%code requires {
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>
#include <iostream>
#include "expression.hh"
#include "statement.hh"
#include "function.hh"
#include "struct.hh"
#include "ast.hh"

extern FILE* yyin;
extern int yylineno;
int yylex();
void yyerror(AST&, const char*);
}

%output "src/parser.cc"
%defines "include/parser.hh"

%parse-param { AST& ast }

%union
{
    std::string* str;
    Expression* expr;
    Statement* stat;
    Function* func;
    std::vector<Statement*>* stats;
    std::vector<Function*>* funcs;
    std::vector<std::string>* strs;
    std::vector<std::vector<std::string>>* vstrs;
    std::vector<Expression*>* exprs;
    std::vector<long long>* numbers;
    std::vector<double>* floats;
    std::vector<std::string>* strings;
    std::vector<std::pair<int, void*>>* fields;
}

%token<str> NUMBER CNUMBER LNUMBER STRING DOUBLE IDENTIFIER TRUE FALSE NULLTOK LEXERROR INT LONG VOID FLOAT BOOL STR CHAR
%token DIVIDE TIMES PLUS MINUS MODULO NOT EQUAL EQUALEQUAL BANGEQUAL LESSEQUAL MOREEQUAL LESSTHAN MORETHAN OR AND
%token SEMICOLON LBRACE RBRACE LPAREN RPAREN LSQUARE RSQUARE COLON COMMA ARROW DOT INCLUDE REQUIRE
%token FUNCTION EXTERN IF ELSE FOR WHILE RETURN LET MUTABLE BREAK STRUCT TEMPLATE SIZEOF

%type<expr>  exp type;
%type<stat>  statement statementblock mutassign;
%type<stats> statements;
%type<strs>  typedarg;
%type<vstrs> typedargs;
%type<exprs> exprlist;
%type<numbers> boolarr intarr;
%type<floats> floatarr;
%type<strings> stringarr;
%type<fields> structfields;
%type<func> function;

%left EQUAL
%left OR AND
%left EQUALEQUAL BANGEQUAL MORETHAN LESSTHAN MOREEQUAL LESSEQUAL
%left PLUS MINUS
%left TIMES DIVIDE
%left UNARY
%left DOT

%destructor { delete $$; } NUMBER STRING DOUBLE IDENTIFIER TRUE FALSE NULLTOK LEXERROR

%%
program: %empty
    | err
    | error
    | program globalfunction
    | program structdef
    | program INCLUDE STRING
    {
        std::string str = std::string(*($3));
        str = str.substr(1, str.size() - 2);
        ast.push_include(str);
    }
    | program REQUIRE STRING
    {
        std::string str = std::string(*($3));
        str = str.substr(1, str.size() - 2);
        ast.push_requires(str);
    }
    ;

structdef: STRUCT IDENTIFIER LBRACE structfields RBRACE
    {
        auto s = new Struct(std::string(*($2)), &ast);
        std::vector<std::pair<int, void*>> vals = *($4);
        for (auto& p : vals) {
            if (p.first == 0) {
                std::tuple<std::string, AtomType, std::string> field = *((std::tuple<std::string, AtomType, std::string>*)p.second);
                if (std::get<1>(field) == t_struct) {
                    s->push_var(std::get<0>(field), std::get<1>(field), std::get<2>(field));
                } else {
                    s->push_var(std::get<0>(field), std::get<1>(field));
                }
            } else {
                s->push_function((Function*)p.second);
            }
        }
        ast.push_struct(s);
    }
    | TEMPLATE IDENTIFIER LBRACE structfields RBRACE
    {
        auto s = new Struct(std::string(*($2)), &ast, true);
        std::vector<std::pair<int, void*>> vals = *($4);
        for (auto& p : vals) {
            if (p.first == 0) {
                std::tuple<std::string, AtomType, std::string> field = *((std::tuple<std::string, AtomType, std::string>*)p.second);
                if (std::get<1>(field) == t_struct) {
                    s->push_var(std::get<0>(field), std::get<1>(field), std::get<2>(field));
                } else {
                    s->push_var(std::get<0>(field), std::get<1>(field));
                }
            } else {
                s->push_function((Function*)p.second);
            }
        }
        ast.push_template(s);
    }
    ;

structfields: %empty
    {
        $$ = new std::vector<std::pair<int, void*>>();
    }
    | structfields function
    {
        $$->push_back(std::pair<int, void*>(1, (void*)$2));
    }
    | structfields IDENTIFIER COLON BOOL COMMA
    {
        std::tuple<std::string, AtomType, std::string>* info = new std::tuple<std::string, AtomType, std::string>(std::string(*($2)), t_char, std::string(""));
        $$->push_back(std::pair<int, void*>(0, (void*)info));
    }
    | structfields IDENTIFIER COLON CHAR COMMA
    {
        std::tuple<std::string, AtomType, std::string>* info = new std::tuple<std::string, AtomType, std::string>(std::string(*($2)), t_char, std::string(""));
        $$->push_back(std::pair<int, void*>(0, (void*)info));
    }
    | structfields IDENTIFIER COLON FLOAT COMMA
    {
        std::tuple<std::string, AtomType, std::string>* info = new std::tuple<std::string, AtomType, std::string>(std::string(*($2)), t_float, std::string(""));
        $$->push_back(std::pair<int, void*>(0, (void*)info));
    }
    | structfields IDENTIFIER COLON INT COMMA
    {
        std::tuple<std::string, AtomType, std::string>* info = new std::tuple<std::string, AtomType, std::string>(std::string(*($2)), t_number, std::string(""));
        $$->push_back(std::pair<int, void*>(0, (void*)info));
    }
    | structfields IDENTIFIER COLON LONG COMMA
    {
        std::tuple<std::string, AtomType, std::string>* info = new std::tuple<std::string, AtomType, std::string>(std::string(*($2)), t_number, std::string(""));
        $$->push_back(std::pair<int, void*>(0, (void*)info));
    }
    | structfields IDENTIFIER COLON STR COMMA
    {
        std::tuple<std::string, AtomType, std::string>* info = new std::tuple<std::string, AtomType, std::string>(std::string(*($2)), t_string, std::string(""));
        $$->push_back(std::pair<int, void*>(0, (void*)info));
    }
    | structfields IDENTIFIER COLON INT TIMES COMMA
    {
        std::tuple<std::string, AtomType, std::string>* info = new std::tuple<std::string, AtomType, std::string>(std::string(*($2)), t_number_arr, std::string(""));
        $$->push_back(std::pair<int, void*>(0, (void*)info));
    }
    | structfields IDENTIFIER COLON FLOAT TIMES COMMA
    {
        std::tuple<std::string, AtomType, std::string>* info = new std::tuple<std::string, AtomType, std::string>(std::string(*($2)), t_float_arr, std::string(""));
        $$->push_back(std::pair<int, void*>(0, (void*)info));
    }
    | structfields IDENTIFIER COLON VOID TIMES COMMA
    {
        std::tuple<std::string, AtomType, std::string>* info = new std::tuple<std::string, AtomType, std::string>(std::string(*($2)), t_string, std::string(""));
        $$->push_back(std::pair<int, void*>(0, (void*)info));
    }
    | structfields IDENTIFIER COLON IDENTIFIER COMMA
    {
        std::tuple<std::string, AtomType, std::string>* info = new std::tuple<std::string, AtomType, std::string>(std::string(*($2)), t_struct, std::string(*($4)));
        $$->push_back(std::pair<int, void*>(0, (void*)info));
    }
    ;

globalfunction: function
    {
        ast.push_function($1);
    }
    ;

function: FUNCTION IDENTIFIER LPAREN RPAREN statementblock
    {
        $$ = new Function(std::string(*($2)), std::move($5), t_null, std::vector<std::vector<std::string>>());
    }
    | FUNCTION IDENTIFIER LPAREN RPAREN ARROW BOOL statementblock
    {
        $$ = new Function(std::string(*($2)), std::move($7), t_bool, std::vector<std::vector<std::string>>());
    }
    | FUNCTION IDENTIFIER LPAREN RPAREN ARROW INT statementblock
    {
        $$ = new Function(std::string(*($2)), std::move($7), t_number, std::vector<std::vector<std::string>>());
    }
    | FUNCTION IDENTIFIER LPAREN RPAREN ARROW LONG statementblock
    {
        $$ = new Function(std::string(*($2)), std::move($7), t_long, std::vector<std::vector<std::string>>());
    }
    | FUNCTION IDENTIFIER LPAREN RPAREN ARROW CHAR statementblock
    {
        $$ = new Function(std::string(*($2)), std::move($7), t_char, std::vector<std::vector<std::string>>());
    }
    | FUNCTION IDENTIFIER LPAREN RPAREN ARROW FLOAT statementblock
    {
        $$ = new Function(std::string(*($2)), std::move($7), t_float, std::vector<std::vector<std::string>>());
    }
    | FUNCTION IDENTIFIER LPAREN RPAREN ARROW STR statementblock
    {
        $$ = new Function(std::string(*($2)), std::move($7), t_string, std::vector<std::vector<std::string>>());
    }
    | FUNCTION IDENTIFIER LPAREN RPAREN ARROW VOID TIMES statementblock
    {
        $$ = new Function(std::string(*($2)), std::move($8), t_string, std::vector<std::vector<std::string>>());
    }
    | FUNCTION IDENTIFIER LPAREN RPAREN ARROW BOOL TIMES statementblock
    {
        $$ = new Function(std::string(*($2)), std::move($8), t_bool_arr, std::vector<std::vector<std::string>>());
    }
    | FUNCTION IDENTIFIER LPAREN RPAREN ARROW INT TIMES statementblock
    {
        $$ = new Function(std::string(*($2)), std::move($8), t_number_arr, std::vector<std::vector<std::string>>());
    }
    | FUNCTION IDENTIFIER LPAREN RPAREN ARROW FLOAT TIMES statementblock
    {
        $$ = new Function(std::string(*($2)), std::move($8), t_float_arr, std::vector<std::vector<std::string>>());
    }
    | FUNCTION IDENTIFIER LPAREN RPAREN ARROW STR TIMES statementblock
    {
        $$ = new Function(std::string(*($2)), std::move($8), t_string_arr, std::vector<std::vector<std::string>>());
    }
    | FUNCTION IDENTIFIER LPAREN RPAREN ARROW IDENTIFIER statementblock
    {
        $$ = new Function(std::string(*($2)), std::move($7), t_struct, std::vector<std::vector<std::string>>(), std::move(*($6)));
    }
    | FUNCTION IDENTIFIER LPAREN typedargs RPAREN statementblock
    {
        $$ = new Function(std::string(*($2)), std::move($6), t_null, std::move(*($4)));
    }
    | FUNCTION IDENTIFIER LPAREN typedargs RPAREN ARROW BOOL statementblock
    {
        $$ = new Function(std::string(*($2)), std::move($8), t_bool, std::move(*($4)));
    }
    | FUNCTION IDENTIFIER LPAREN typedargs RPAREN ARROW INT statementblock
    {
        $$ = new Function(std::string(*($2)), std::move($8), t_number, std::move(*($4)));
    }
    | FUNCTION IDENTIFIER LPAREN typedargs RPAREN ARROW LONG statementblock
    {
        $$ = new Function(std::string(*($2)), std::move($8), t_long, std::move(*($4)));
    }
    | FUNCTION IDENTIFIER LPAREN typedargs RPAREN ARROW CHAR statementblock
    {
        $$ = new Function(std::string(*($2)), std::move($8), t_char, std::move(*($4)));
    }
    | FUNCTION IDENTIFIER LPAREN typedargs RPAREN ARROW FLOAT statementblock
    {
        $$ = new Function(std::string(*($2)), std::move($8), t_float, std::move(*($4)));
    }
    | FUNCTION IDENTIFIER LPAREN typedargs RPAREN ARROW STR statementblock
    {
        $$ = new Function(std::string(*($2)), std::move($8), t_string, std::move(*($4)));
    }
    | FUNCTION IDENTIFIER LPAREN typedargs RPAREN ARROW VOID TIMES statementblock
    {
        $$ = new Function(std::string(*($2)), std::move($9), t_string, std::move(*($4)));
    }
    | FUNCTION IDENTIFIER LPAREN typedargs RPAREN ARROW BOOL TIMES statementblock
    {
        $$ = new Function(std::string(*($2)), std::move($9), t_bool_arr, std::move(*($4)));
    }
    | FUNCTION IDENTIFIER LPAREN typedargs RPAREN ARROW INT TIMES statementblock
    {
        $$ = new Function(std::string(*($2)), std::move($9), t_number_arr, std::move(*($4)));
    }
    | FUNCTION IDENTIFIER LPAREN typedargs RPAREN ARROW FLOAT TIMES statementblock
    {
        $$ = new Function(std::string(*($2)), std::move($9), t_float_arr, std::move(*($4)));
    }
    | FUNCTION IDENTIFIER LPAREN typedargs RPAREN ARROW STR TIMES statementblock
    {
        $$ = new Function(std::string(*($2)), std::move($9), t_string_arr, std::move(*($4)));
    }
    | FUNCTION IDENTIFIER LPAREN typedargs RPAREN ARROW IDENTIFIER statementblock
    {
        $$ = new Function(std::string(*($2)), std::move($8), t_struct, std::move(*($4)), std::move(*($7)));
    }
    | EXTERN IDENTIFIER LPAREN RPAREN SEMICOLON
    {
        $$ = new Function(std::string(*($2)), nullptr, t_null, std::vector<std::vector<std::string>>());
    }
    | EXTERN IDENTIFIER LPAREN RPAREN ARROW BOOL SEMICOLON
    {
        $$ = new Function(std::string(*($2)), nullptr, t_bool, std::vector<std::vector<std::string>>());
    }
    | EXTERN IDENTIFIER LPAREN RPAREN ARROW INT SEMICOLON
    {
        $$ = new Function(std::string(*($2)), nullptr, t_number, std::vector<std::vector<std::string>>());
    }
    | EXTERN IDENTIFIER LPAREN RPAREN ARROW LONG SEMICOLON
    {
        $$ = new Function(std::string(*($2)), nullptr, t_long, std::vector<std::vector<std::string>>());
    }
    | EXTERN IDENTIFIER LPAREN RPAREN ARROW CHAR SEMICOLON
    {
        $$ = new Function(std::string(*($2)), nullptr, t_char, std::vector<std::vector<std::string>>());
    }
    | EXTERN IDENTIFIER LPAREN RPAREN ARROW FLOAT SEMICOLON
    {
        $$ = new Function(std::string(*($2)), nullptr, t_float, std::vector<std::vector<std::string>>());
    }
    | EXTERN IDENTIFIER LPAREN RPAREN ARROW STR SEMICOLON
    {
        $$ = new Function(std::string(*($2)), nullptr, t_string, std::vector<std::vector<std::string>>());
    }
    | EXTERN IDENTIFIER LPAREN RPAREN ARROW VOID TIMES SEMICOLON
    {
        $$ = new Function(std::string(*($2)), nullptr, t_string, std::vector<std::vector<std::string>>());
    }
    | EXTERN IDENTIFIER LPAREN RPAREN ARROW BOOL TIMES SEMICOLON
    {
        $$ = new Function(std::string(*($2)), nullptr, t_bool_arr, std::vector<std::vector<std::string>>());
    }
    | EXTERN IDENTIFIER LPAREN RPAREN ARROW INT TIMES SEMICOLON
    {
        $$ = new Function(std::string(*($2)), nullptr, t_number_arr, std::vector<std::vector<std::string>>());
    }
    | EXTERN IDENTIFIER LPAREN RPAREN ARROW FLOAT TIMES SEMICOLON
    {
        $$ = new Function(std::string(*($2)), nullptr, t_float_arr, std::vector<std::vector<std::string>>());
    }
    | EXTERN IDENTIFIER LPAREN RPAREN ARROW STR TIMES SEMICOLON
    {
        $$ = new Function(std::string(*($2)), nullptr, t_string_arr, std::vector<std::vector<std::string>>());
    }
    | EXTERN IDENTIFIER LPAREN typedargs RPAREN SEMICOLON
    {
        $$ = new Function(std::string(*($2)), nullptr, t_null, std::move(*($4)));
    }
    | EXTERN IDENTIFIER LPAREN typedargs RPAREN ARROW BOOL SEMICOLON
    {
        $$ = new Function(std::string(*($2)), nullptr, t_bool, std::move(*($4)));
    }
    | EXTERN IDENTIFIER LPAREN typedargs RPAREN ARROW INT SEMICOLON
    {
        $$ = new Function(std::string(*($2)), nullptr, t_number, std::move(*($4)));
    }
    | EXTERN IDENTIFIER LPAREN typedargs RPAREN ARROW LONG SEMICOLON
    {
        $$ = new Function(std::string(*($2)), nullptr, t_long, std::move(*($4)));
    }
    | EXTERN IDENTIFIER LPAREN typedargs RPAREN ARROW CHAR SEMICOLON
    {
        $$ = new Function(std::string(*($2)), nullptr, t_char, std::move(*($4)));
    }
    | EXTERN IDENTIFIER LPAREN typedargs RPAREN ARROW FLOAT SEMICOLON
    {
        $$ = new Function(std::string(*($2)), nullptr, t_float, std::move(*($4)));
    }
    | EXTERN IDENTIFIER LPAREN typedargs RPAREN ARROW STR SEMICOLON
    {
        $$ = new Function(std::string(*($2)), nullptr, t_string, std::move(*($4)));
    }
    | EXTERN IDENTIFIER LPAREN typedargs RPAREN ARROW VOID TIMES SEMICOLON
    {
        $$ = new Function(std::string(*($2)), nullptr, t_string, std::move(*($4)));
    }
    | EXTERN IDENTIFIER LPAREN typedargs RPAREN ARROW BOOL TIMES SEMICOLON
    {
        $$ = new Function(std::string(*($2)), nullptr, t_bool_arr, std::move(*($4)));
    }
    | EXTERN IDENTIFIER LPAREN typedargs RPAREN ARROW INT TIMES SEMICOLON
    {
        $$ = new Function(std::string(*($2)), nullptr, t_number_arr, std::move(*($4)));
    }
    | EXTERN IDENTIFIER LPAREN typedargs RPAREN ARROW FLOAT TIMES SEMICOLON
    {
        $$ = new Function(std::string(*($2)), nullptr, t_float_arr, std::move(*($4)));
    }
    | EXTERN IDENTIFIER LPAREN typedargs RPAREN ARROW STR TIMES SEMICOLON
    {
        $$ = new Function(std::string(*($2)), nullptr, t_string_arr, std::move(*($4)));
    }
    ;

statementblock: LBRACE statements RBRACE
    {
        $$ = new StatementBlock(std::move(*($2)));
    }
    ;

statements: %empty
    {
        $$ = new std::vector<Statement*>();
    }
    | statements statement
    {
        $$->push_back(std::move($2));
    }
    | statements statementblock
    {
        $$->push_back(std::move($2));
    }
    ;

statement: exp SEMICOLON
    {
        $$ = new ExpressionStatement(std::move($1));
    }
    | RETURN exp SEMICOLON
    {
        $$ = new ReturnStatement(std::move($2));
    }
    | RETURN SEMICOLON
    {
        $$ = new ReturnStatement(nullptr);
    }
    | LET exp COLON INT EQUAL exp SEMICOLON
    {
        $$ = new AssignmentStatement(std::move($2), std::move($6), false, t_number);
    }
    | LET exp COLON LONG EQUAL exp SEMICOLON
    {
        $$ = new AssignmentStatement(std::move($2), std::move($6), false, t_long);
    }
    | LET exp COLON CHAR EQUAL exp SEMICOLON
    {
        $$ = new AssignmentStatement(std::move($2), std::move($6), false, t_char);
    }
    | LET exp COLON FLOAT EQUAL exp SEMICOLON
    {
        $$ = new AssignmentStatement(std::move($2), std::move($6), false, t_float);
    }
    | LET exp COLON BOOL EQUAL exp SEMICOLON
    {
        $$ = new AssignmentStatement(std::move($2), std::move($6), false, t_bool);
    }
    | LET exp COLON STR EQUAL exp SEMICOLON
    {
        $$ = new AssignmentStatement(std::move($2), std::move($6), false, t_string);
    }
    | LET exp COLON INT TIMES EQUAL exp SEMICOLON
    {
        $$ = new AssignmentStatement(std::move($2), std::move($7), false, t_number_arr);
    }
    | LET exp COLON FLOAT TIMES EQUAL exp SEMICOLON
    {
        $$ = new AssignmentStatement(std::move($2), std::move($7), false, t_float_arr);
    }
    | LET exp COLON BOOL TIMES EQUAL exp SEMICOLON
    {
        $$ = new AssignmentStatement(std::move($2), std::move($7), false, t_bool_arr);
    }
    | LET exp COLON STR TIMES EQUAL exp SEMICOLON
    {
        $$ = new AssignmentStatement(std::move($2), std::move($7), false, t_string_arr);
    }
    | LET exp COLON VOID TIMES EQUAL exp SEMICOLON
    {
        $$ = new AssignmentStatement(std::move($2), std::move($7), false, t_string);
    }
    | mutassign
    {
        $$ = std::move($1);
    }
    | IF exp statementblock
    {
        $$ = new IfStatement(std::move($2), std::move($3), nullptr);
    }
    | IF exp statementblock ELSE statement
    {
        $$ = new IfStatement(std::move($2), std::move($3), std::move($5));
    }
    | IF exp statementblock ELSE statementblock
    {
        $$ = new IfStatement(std::move($2), std::move($3), std::move($5));
    }
    | WHILE exp statementblock
    {
        $$ = new WhileStatement(std::move($2), std::move($3));
    }
    | FOR LPAREN mutassign exp SEMICOLON exp RPAREN statementblock
    {
        StatementBlock* s = (StatementBlock*)std::move($8);
        s->statements.push_back(new ExpressionStatement(std::move($6)));
        WhileStatement* a = new WhileStatement(std::move($4), std::move(s));
        std::vector<Statement*> stats = std::vector<Statement*>();
        stats.push_back(std::move($3));
        stats.push_back(a);
        $$ = new StatementBlock(stats);
    }
    ;

mutassign: LET MUTABLE exp COLON INT EQUAL exp SEMICOLON
    {
        $$ = new AssignmentStatement(std::move($3), std::move($7), true, t_number);
    }
    | LET MUTABLE exp COLON LONG EQUAL exp SEMICOLON
    {
        $$ = new AssignmentStatement(std::move($3), std::move($7), true, t_long);
    }
    | LET MUTABLE exp COLON FLOAT EQUAL exp SEMICOLON
    {
        $$ = new AssignmentStatement(std::move($3), std::move($7), true, t_float);
    }
    | LET MUTABLE exp COLON BOOL EQUAL exp SEMICOLON
    {
        $$ = new AssignmentStatement(std::move($3), std::move($7), true, t_bool);
    }
    | LET MUTABLE exp COLON STR EQUAL exp SEMICOLON
    {
        $$ = new AssignmentStatement(std::move($3), std::move($7), true, t_string);
    }
    | LET MUTABLE exp COLON CHAR EQUAL exp SEMICOLON
    {
        $$ = new AssignmentStatement(std::move($3), std::move($7), true, t_char);
    }
    | LET MUTABLE exp COLON IDENTIFIER EQUAL exp SEMICOLON
    {
        $$ = new AssignmentStatement(std::move($3), std::move($7), true, t_struct, std::string(*($5)));
    }
    | LET MUTABLE exp COLON INT TIMES EQUAL exp SEMICOLON
    {
        $$ = new AssignmentStatement(std::move($3), std::move($8), true, t_number_arr);
    }
    | LET MUTABLE exp COLON FLOAT TIMES EQUAL exp SEMICOLON
    {
        $$ = new AssignmentStatement(std::move($3), std::move($8), true, t_float_arr);
    }
    | LET MUTABLE exp COLON BOOL TIMES EQUAL exp SEMICOLON
    {
        $$ = new AssignmentStatement(std::move($3), std::move($8), true, t_bool_arr);
    }
    | LET MUTABLE exp COLON STR TIMES EQUAL exp SEMICOLON
    {
        $$ = new AssignmentStatement(std::move($3), std::move($8), true, t_string_arr);
    }
    | LET MUTABLE exp COLON VOID TIMES EQUAL exp SEMICOLON
    {
        $$ = new AssignmentStatement(std::move($3), std::move($8), true, t_string);
    }
    | LET MUTABLE exp COLON INT LSQUARE NUMBER RSQUARE SEMICOLON
    {
        $$ = new AssignmentStatement(std::move($3), nullptr, true, t_number_arr, std::stol(*($7)));
    }
    | LET MUTABLE exp COLON FLOAT LSQUARE NUMBER RSQUARE SEMICOLON
    {
        $$ = new AssignmentStatement(std::move($3), nullptr, true, t_float_arr, std::stol(*($7)));
    }
    | LET MUTABLE exp COLON BOOL LSQUARE NUMBER RSQUARE SEMICOLON
    {
        $$ = new AssignmentStatement(std::move($3), nullptr, true, t_bool_arr, std::stol(*($7)));
    }
    | LET MUTABLE exp COLON STR LSQUARE NUMBER RSQUARE SEMICOLON
    {
        $$ = new AssignmentStatement(std::move($3), nullptr, true, t_string_arr, std::stol(*($7)));
    }
    ;

typedargs: %empty
    {
        $$ = new std::vector<std::vector<std::string>>();
    }
    | typedargs typedarg
    {
        if ($$->size() > 0) {
            yyerror(ast, "Expected a comma between arguments.");
        } else {
            $$->push_back(std::move(*($2)));
        }
    }
    | typedargs COMMA typedarg
    {
        $$->push_back(std::move(*($3)));
    }
    ;

typedarg: IDENTIFIER COLON INT
    {
        $$ = new std::vector<std::string>();
        $$->push_back(std::string(*($1)));
        $$->push_back(std::string("int"));
        $$->push_back(std::string("f"));
        delete $1;
    }
    | IDENTIFIER COLON LONG
    {
        $$ = new std::vector<std::string>();
        $$->push_back(std::string(*($1)));
        $$->push_back(std::string("long"));
        $$->push_back(std::string("f"));
        delete $1;
    }

    | IDENTIFIER COLON FLOAT
    {
        $$ = new std::vector<std::string>();
        $$->push_back(std::string(*($1)));
        $$->push_back(std::string("float"));
        $$->push_back(std::string("f"));
        delete $1;
    }
    | IDENTIFIER COLON BOOL
    {
        $$ = new std::vector<std::string>();
        $$->push_back(std::string(*($1)));
        $$->push_back(std::string("bool"));
        $$->push_back(std::string("f"));
        delete $1;
    }
    | IDENTIFIER COLON INT TIMES
    {
        $$ = new std::vector<std::string>();
        $$->push_back(std::string(*($1)));
        $$->push_back(std::string("intarr"));
        $$->push_back(std::string("f"));
        delete $1;
    }
    | IDENTIFIER COLON IDENTIFIER
    {
        $$ = new std::vector<std::string>();
        $$->push_back(std::string(*($1)));
        $$->push_back(std::string("struct"));
        $$->push_back(std::string("t"));
        $$->push_back(std::string(*($3)));
        delete $1;
        delete $3;
    }
    | IDENTIFIER COLON FLOAT TIMES
    {
        $$ = new std::vector<std::string>();
        $$->push_back(std::string(*($1)));
        $$->push_back(std::string("floatarr"));
        $$->push_back(std::string("f"));
        delete $1;
    }
    | IDENTIFIER COLON BOOL TIMES
    {
        $$ = new std::vector<std::string>();
        $$->push_back(std::string(*($1)));
        $$->push_back(std::string("boolarr"));
        $$->push_back(std::string("f"));
        delete $1;
    }
    | IDENTIFIER COLON STR TIMES
    {
        $$ = new std::vector<std::string>();
        $$->push_back(std::string(*($1)));
        $$->push_back(std::string("stringarr"));
        $$->push_back(std::string("f"));
        delete $1;
    }
    | IDENTIFIER COLON VOID TIMES
    {
        $$ = new std::vector<std::string>();
        $$->push_back(std::string(*($1)));
        $$->push_back(std::string("string"));
        $$->push_back(std::string("f"));
        delete $1;
    }
    | IDENTIFIER COLON STR
    {
        $$ = new std::vector<std::string>();
        $$->push_back(std::string(*($1)));
        $$->push_back(std::string("string"));
        $$->push_back(std::string("f"));
        delete $1;
    }
    | IDENTIFIER COLON CHAR
    {
        $$ = new std::vector<std::string>();
        $$->push_back(std::string(*($1)));
        $$->push_back(std::string("char"));
        $$->push_back(std::string("f"));
        delete $1;
    }
    | MUTABLE IDENTIFIER COLON INT
    {
        $$ = new std::vector<std::string>();
        $$->push_back(std::string(*($2)));
        $$->push_back(std::string("int"));
        $$->push_back(std::string("t"));
        delete $2;
    }
    | MUTABLE IDENTIFIER COLON LONG
    {
        $$ = new std::vector<std::string>();
        $$->push_back(std::string(*($2)));
        $$->push_back(std::string("long"));
        $$->push_back(std::string("t"));
        delete $2;
    }
    | MUTABLE IDENTIFIER COLON FLOAT
    {
        $$ = new std::vector<std::string>();
        $$->push_back(std::string(*($2)));
        $$->push_back(std::string("float"));
        $$->push_back(std::string("t"));
        delete $2;
    }
    | MUTABLE IDENTIFIER COLON BOOL
    {
        $$ = new std::vector<std::string>();
        $$->push_back(std::string(*($2)));
        $$->push_back(std::string("bool"));
        $$->push_back(std::string("t"));
        delete $2;
    }
    | MUTABLE IDENTIFIER COLON IDENTIFIER
    {
        $$ = new std::vector<std::string>();
        $$->push_back(std::string(*($2)));
        $$->push_back(std::string("struct"));
        $$->push_back(std::string("t"));
        $$->push_back(std::string(*($4)));
        delete $2;
        delete $4;
    }
    | MUTABLE IDENTIFIER COLON INT TIMES
    {
        $$ = new std::vector<std::string>();
        $$->push_back(std::string(*($2)));
        $$->push_back(std::string("intarr"));
        $$->push_back(std::string("t"));
        delete $2;
    }
    | MUTABLE IDENTIFIER COLON FLOAT TIMES
    {
        $$ = new std::vector<std::string>();
        $$->push_back(std::string(*($2)));
        $$->push_back(std::string("floatarr"));
        $$->push_back(std::string("t"));
        delete $2;
    }
    | MUTABLE IDENTIFIER COLON BOOL TIMES
    {
        $$ = new std::vector<std::string>();
        $$->push_back(std::string(*($2)));
        $$->push_back(std::string("boolarr"));
        $$->push_back(std::string("t"));
        delete $2;
    }
    | MUTABLE IDENTIFIER COLON STR TIMES
    {
        $$ = new std::vector<std::string>();
        $$->push_back(std::string(*($2)));
        $$->push_back(std::string("stringarr"));
        $$->push_back(std::string("t"));
        delete $2;
    }
    | MUTABLE IDENTIFIER COLON STR
    {
        $$ = new std::vector<std::string>();
        $$->push_back(std::string(*($2)));
        $$->push_back(std::string("string"));
        $$->push_back(std::string("t"));
        delete $2;
    }
    | MUTABLE IDENTIFIER COLON VOID TIMES
    {
        $$ = new std::vector<std::string>();
        $$->push_back(std::string(*($2)));
        $$->push_back(std::string("string"));
        $$->push_back(std::string("t"));
        delete $2;
    }
    | MUTABLE IDENTIFIER COLON CHAR
    {
        $$ = new std::vector<std::string>();
        $$->push_back(std::string(*($2)));
        $$->push_back(std::string("char"));
        $$->push_back(std::string("t"));
        delete $2;
    }
    ;

exprlist: %empty
    {
        $$ = new std::vector<Expression*>();
    }
    | exprlist exp
    {
        if ($$->size() > 0) {
            yyerror(ast, "Expected a comma between expressions.");
        } else {
            $$->push_back($2);
        }
    }
    | exprlist COMMA exp
    {
        $$->push_back($3);
    }
    ;

type: INT
    {
        $$ = new ExpressionAtomic(std::string(*($1)), true);
        free($1);
    }
    | IDENTIFIER
    {
        $$ = new ExpressionAtomic(std::string(*($1)), true);
        free($1);
    }
    | LONG
    {
        $$ = new ExpressionAtomic(std::string(*($1)), true);
        free($1);
    }
    | VOID
    {
        $$ = new ExpressionAtomic(std::string(*($1)), true);
        free($1);
    }
    | FLOAT
    {
        $$ = new ExpressionAtomic(std::string(*($1)), true);
        free($1);
    }
    | BOOL
    {
        $$ = new ExpressionAtomic(std::string(*($1)), true);
        free($1);
    }
    | CHAR
    {
        $$ = new ExpressionAtomic(std::string(*($1)), true);
        free($1);
    }
    | STR
    {
        $$ = new ExpressionAtomic(std::string(*($1)), true);
        free($1);
    }
    | INT TIMES
    {
        $$ = new ExpressionAtomic(std::string("foo*"), true);
        free($1);
    }
    | LONG TIMES
    {
        $$ = new ExpressionAtomic(std::string("foo*"), true);
        free($1);
    }
    | VOID TIMES
    {
        $$ = new ExpressionAtomic(std::string("foo*"), true);
        free($1);
    }
    | FLOAT TIMES
    {
        $$ = new ExpressionAtomic(std::string("foo*"), true);
        free($1);
    }
    | BOOL TIMES
    {
        $$ = new ExpressionAtomic(std::string("foo*"), true);
        free($1);
    }
    | CHAR TIMES
    {
        $$ = new ExpressionAtomic(std::string("foo*"), true);
        free($1);
    }
    | STR TIMES
    {
        $$ = new ExpressionAtomic(std::string("foo*"), true);
        free($1);
    }
    | IDENTIFIER TIMES
    {
        $$ = new ExpressionAtomic(std::string("foo*"), true);
        free($1);
    }

exp: NUMBER
    {
        $$ = new ExpressionAtomic(std::stol(*($1)));
        free($1);
    }
    | LNUMBER
    {
        $$ = new ExpressionAtomic(std::stoll(*($1)));
        free($1);
    }
    | CNUMBER
    {
        std::string a = *($1);
        $$ = new ExpressionAtomic((char)std::stoll(a.substr(1)));
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
    | IDENTIFIER LPAREN RPAREN
    {
        $$ = new ExpressionAtomic(std::string(*($1)), std::vector<Expression*>());
        free($1);
    }
    | SIZEOF LPAREN type RPAREN
    {
        auto x = std::vector<Expression*>();
        x.push_back($3);
        $$ = new ExpressionAtomic(std::string("sizeof"), x);
    }
    | IDENTIFIER LPAREN exprlist RPAREN
    {
        $$ = new ExpressionAtomic(std::string(*($1)), std::move(*($3)));
        free($1);
    }
    | IDENTIFIER
    {
        $$ = new ExpressionAtomic(std::string(*($1)), true);
        free($1);
    }
    | IDENTIFIER LSQUARE exp RSQUARE
    {
        $$ = new ExpressionAtomic(std::string(*($1)), std::move($3));
        free($1);
    }
    | BREAK
    {
        $$ = new BreakExpression();
    }
    | LBRACE boolarr RBRACE
    {
        $$ = new ExpressionAtomic(t_bool_arr, (*($2)).size(), std::move(*($2)));
    }
    | LBRACE intarr RBRACE
    {
        $$ = new ExpressionAtomic(t_number_arr, (*($2)).size(), std::move(*($2)));
    }
    | LBRACE floatarr RBRACE
    {
        $$ = new ExpressionAtomic(t_float_arr, (*($2)).size(), std::move(*($2)));
    }
    | LBRACE stringarr RBRACE
    {
        $$ = new ExpressionAtomic((*($2)).size(), std::move(*($2)));
    }
    | exp DOT IDENTIFIER
    {
        $$ = new ExpressionAtomic(std::move($1), new ExpressionAtomic(std::string(*($3)), true));
    }
    | exp DOT IDENTIFIER LPAREN RPAREN
    {
        $$ = new ExpressionAtomic(std::move($1), new ExpressionAtomic(std::string(*($3)), std::vector<Expression*>()));
    }
    | exp DOT IDENTIFIER LPAREN exprlist RPAREN
    {
        $$ = new ExpressionAtomic(std::move($1), new ExpressionAtomic(std::string(*($3)), std::move(*($5))));
    }
    ;


exp: exp PLUS exp
    {
        $$ = new BinaryExpression(std::move($1), std::move($3), op_add);
    }
    | exp MINUS exp
    {
        $$ = new BinaryExpression(std::move($1), std::move($3), op_sub);
    }
    | exp TIMES exp
    {
        $$ = new BinaryExpression(std::move($1), std::move($3), op_mul);
    }
    | exp DIVIDE exp
    {
        $$ = new BinaryExpression(std::move($1), std::move($3), op_div);
    }
    | exp MORETHAN exp
    {
        $$ = new BinaryExpression(std::move($1), std::move($3), op_greater);
    }
    | exp LESSTHAN exp
    {
        $$ = new BinaryExpression(std::move($1), std::move($3), op_less);
    }
    | exp MOREEQUAL exp
    {
        $$ = new BinaryExpression(std::move($1), std::move($3), op_greater_equal);
    }
    | exp LESSEQUAL exp
    {
        $$ = new BinaryExpression(std::move($1), std::move($3), op_less_equal);
    }
    | exp EQUALEQUAL exp
    {
        $$ = new BinaryExpression(std::move($1), std::move($3), op_equal);
    }
    | exp BANGEQUAL exp
    {
        $$ = new BinaryExpression(std::move($1), std::move($3), op_not_equal);
    }
    | exp AND exp
    {
        $$ = new BinaryExpression(std::move($1), std::move($3), op_and);
    }
    | exp OR exp
    {
        $$ = new BinaryExpression(std::move($1), std::move($3), op_or);
    }
    | exp MODULO exp
    {
        $$ = new BinaryExpression(std::move($1), std::move($3), op_modulo);
    }
    | exp EQUAL exp
    {
        if (($1)->get_atomic_type_keep_identifier(&ast) == t_identifier) {
            $$ = new AssignmentExpression(std::move($1), std::move($3));
        } else {
            $$ = new AssignmentExpression(std::move($1), std::move($3), 1);
        }
    }
    | IDENTIFIER LSQUARE exp RSQUARE EQUAL exp
    {
        $$ = new AssignmentExpression(new ExpressionAtomic(std::string(*($1)), true), std::move($6), std::move($3));
        free($1);
    }
    ;


exp: NOT exp %prec UNARY
    {
        $$ = new UnaryExpression(std::move($2), std::string("!"));
    }
    | MINUS exp %prec UNARY
    {
        $$ = new UnaryExpression(std::move($2), std::string("-"));
    }
    | exp PLUS PLUS %prec UNARY
    {
        $$ = new UnaryExpression(std::move($1), std::string("++"));
    }
    | LPAREN exp RPAREN
    {
        $$ = new UnaryExpression(std::move($2), std::string("()"));
    }
    ;

boolarr: %empty
    {
        $$ = new std::vector<long long>();
    }
    | boolarr TRUE
    {
        if ($$->size() > 0) {
            yyerror(ast, "Expected a comma between expressions.");
        } else {
            $$->push_back(1);
        }
        free($2);
    }
    | boolarr FALSE
    {
        if ($$->size() > 0) {
            yyerror(ast, "Expected a comma between expressions.");
        } else {
            $$->push_back(0);
        }
        free($2);
    }
    | boolarr COMMA TRUE
    {
        $$->push_back(1);
        free($3);
    }
    | boolarr COMMA FALSE
    {
        $$->push_back(0);
        free($3);
    }
    ;

intarr: %empty
    {
        $$ = new std::vector<long long>();
    }
    | intarr NUMBER
    {
        if ($$->size() > 0) {
            yyerror(ast, "Expected a comma between expressions.");
        } else {
            $$->push_back(std::stoll(*($2)));
        }
        free($2);
    }
    | intarr COMMA NUMBER
    {
        $$->push_back(std::stoll(*($3)));
        free($3);
    }
    ;

floatarr: %empty
    {
        $$ = new std::vector<double>();
    }
    | floatarr DOUBLE
    {
        if ($$->size() > 0) {
            yyerror(ast, "Expected a comma between expressions.");
        } else {
            $$->push_back(std::stod(*($2)));
        }
        free($2);
    }
    | floatarr COMMA DOUBLE
    {
        $$->push_back(std::stod(*($3)));
        free($3);
    }
    ;

stringarr: %empty
    {
        $$ = new std::vector<std::string>();
    }
    | stringarr STRING
    {
        if ($$->size() > 0) {
            yyerror(ast, "Expected a comma between expressions.");
        } else {
            $$->push_back(std::move(*($2)));
        }
    }
    | stringarr COMMA STRING
    {
        $$->push_back(std::move(*($3)));
    }
    ;

err: LEXERROR
     {
        ast.push_err(std::string(*($1)));
     }
     ;

