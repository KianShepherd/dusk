%{
#include <string>
#include "parser.h"
%}

%option noinput nounput noyywrap nodefault
%option yylineno
%option outfile="src/scanner.cc"
%%
[0-9]+          { yylval = new std::string(yytext, yyleng); return NUMBER; }
\"(.*?)\"       { yylval = new std::string(yytext, yyleng); return STRING; }
[0-9]+\.[0-9]+  { yylval = new std::string(yytext, yyleng); return DOUBLE; }
[ \t\r\n]+      /* eat whitespace */;
\+              { yylval = new std::string(yytext, yyleng); return PLUS; }
\-              { yylval = new std::string(yytext, yyleng); return MINUS; }
\*              { yylval = new std::string(yytext, yyleng); return TIMES; }
\/              { yylval = new std::string(yytext, yyleng); return DIVIDE; }
;               { yylval = new std::string(yytext, yyleng); return SEMICOLON; }
{               { yylval = new std::string(yytext, yyleng); return RBRACE; }
}               { yylval = new std::string(yytext, yyleng); return LBRACE; }
\(              { yylval = new std::string(yytext, yyleng); return RPAREN; }
\)              { yylval = new std::string(yytext, yyleng); return LPAREN; }
:               { yylval = new std::string(yytext, yyleng); return COLON; }
%%
