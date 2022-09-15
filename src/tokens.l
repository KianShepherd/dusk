%{
#include <stdio.h>
#include "y.tab.hh"

extern YYSTYPE yylval;
%}

%%
[0-9]+          yylval.number=atoi(yytext); return NUMBER;
\"[a-zA-Z ]+\"  yylval.string=strdup(yytext); return STRING;
[ \n]+          /* eat whitespace */;
\+              return PLUS;
\-              return MINUS;
\*              return MULT;
\/              return DIV;
;               return SEMICOLON;
.               return (int) yytext[0];
%%
