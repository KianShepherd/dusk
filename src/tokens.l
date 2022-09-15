%{
#include <stdio.h>
#include "y.tab.hh"

extern YYSTYPE yylval;
%}

%%
[0-9]+          yylval.number=atoi(yytext); return NUMBER;
\"[a-zA-Z ]+\"  yylval.string=strdup(yytext); return STRING;
[ \t\r\n]+          /* eat whitespace */;
\+              return PLUS;
\-              return MINUS;
\*              return TIMES;
\/              return DIVIDE;
;               return SEMICOLON;
{               return RBRACE;
}               return LBRACE;
\(              return RPAREN;
\)              return LPAREN;
:               return COLON;
%%
