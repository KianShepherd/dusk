/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_SRC_PARSER_H_INCLUDED
# define YY_YY_SRC_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "src/grammar.y"

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

#line 65 "src/parser.h"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NUMBER = 258,
    STRING = 259,
    DOUBLE = 260,
    IDENTIFIER = 261,
    TRUE = 262,
    FALSE = 263,
    NULLTOK = 264,
    DIVIDE = 265,
    TIMES = 266,
    PLUS = 267,
    MINUS = 268,
    NOT = 269,
    EQUAL = 270,
    EQUALEQUAL = 271,
    BANGEQUAL = 272,
    LESSEQUAL = 273,
    MOREEQUAL = 274,
    LESSTHAN = 275,
    MORETHAN = 276,
    OR = 277,
    AND = 278,
    SEMICOLON = 279,
    LBRACE = 280,
    RBRACE = 281,
    LPAREN = 282,
    RPAREN = 283,
    COLON = 284,
    COMMA = 285,
    ARROW = 286,
    INT = 287,
    VOID = 288,
    FLOAT = 289,
    BOOL = 290,
    STR = 291,
    FUNCTION = 292,
    IF = 293,
    ELSE = 294,
    FOR = 295,
    WHILE = 296,
    RETURN = 297,
    LET = 298,
    MUTABLE = 299,
    LEXERROR = 300,
    UNARY = 301
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 24 "src/grammar.y"

    std::string* str;
    Expression* expr;
    Statement* stat;
    Function* func;
    std::vector<Statement*>* stats;
    std::vector<Function*>* funcs;

#line 132 "src/parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (AST& ast);

#endif /* !YY_YY_SRC_PARSER_H_INCLUDED  */
