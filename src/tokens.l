%{
#include <string>
#include "parser.h"
%}

%option noinput nounput noyywrap nodefault
%option yylineno
%option outfile="src/scanner.cc"
%%
[0-9]+              { yylval.str = new std::string(yytext, yyleng); return NUMBER; }
[0-9]+\.[0-9]+      { yylval.str = new std::string(yytext, yyleng); return DOUBLE; }
\"([^\\\"]|\\.)*\"  { yylval.str = new std::string(yytext, yyleng); return STRING; }

[[:space:]]         ; /* Ignore whitespace */
\+                  { yylval.str = new std::string(yytext, yyleng); return PLUS; }
\-                  { yylval.str = new std::string(yytext, yyleng); return MINUS; }
\*                  { yylval.str = new std::string(yytext, yyleng); return TIMES; }
\/                  { yylval.str = new std::string(yytext, yyleng); return DIVIDE; }
;                   { yylval.str = new std::string(yytext, yyleng); return SEMICOLON; }
{                   { yylval.str = new std::string(yytext, yyleng); return RBRACE; }
}                   { yylval.str = new std::string(yytext, yyleng); return LBRACE; }
\(                  { yylval.str = new std::string(yytext, yyleng); return RPAREN; }
\)                  { yylval.str = new std::string(yytext, yyleng); return LPAREN; }
:                   { yylval.str = new std::string(yytext, yyleng); return COLON; }
%%
