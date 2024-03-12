%{
#include <string>
#include <cstdio>
#include <sstream>
#include "parser.hh"
/*

%token<str> IDENTIFIER
*/
%}

%option noinput nounput noyywrap nodefault
%option yylineno
%option outfile="src/scanner.cc"

%x comment
%%
[0-9]+                  { yylval.str = new std::string(yytext, yyleng); return NUMBER;  }
[0-9]+c                 { yylval.str = new std::string(yytext, yyleng); return CNUMBER;  }
[0-9]+l                 { yylval.str = new std::string(yytext, yyleng); return LNUMBER;  }
[0-9]+\.[0-9]+          { yylval.str = new std::string(yytext, yyleng); return DOUBLE;  }
\"([^\\\"]|\\.)*\"      { yylval.str = new std::string(yytext, yyleng); return STRING;  }
True                    { yylval.str = new std::string(yytext, yyleng); return TRUE;    }
False                   { yylval.str = new std::string(yytext, yyleng); return FALSE;   }
Null                    { yylval.str = new std::string(yytext, yyleng); return NULLTOK; }

\+                      { return PLUS;       }
\-                      { return MINUS;      }
\*                      { return TIMES;      }
\/                      { return DIVIDE;     }
\%                      { return MODULO;     }
==                      { return EQUALEQUAL; }
!=                      { return BANGEQUAL;  }
\<=                     { return LESSEQUAL;  }
>=                      { return MOREEQUAL;  }
=                       { return EQUAL;      }
\<                      { return LESSTHAN;   }
>                       { return MORETHAN;   }
\|\|                    { return OR;         }
&&                      { return AND;        }
!                       { return NOT;        }

;                       { return SEMICOLON; }
\{                      { return LBRACE;    }
\}                      { return RBRACE;    }
\(                      { return LPAREN;    }
\)                      { return RPAREN;    }
\[                      { return LSQUARE;   }
\]                      { return RSQUARE;   }
:                       { return COLON;     }
,                       { return COMMA;     }
\.                      { return DOT;       }
->                      { return ARROW;     }

int                     { yylval.str = new std::string(yytext, yyleng); return INT;   }
sizeof                  { yylval.str = new std::string(yytext, yyleng); return SIZEOF;   }
long                    { yylval.str = new std::string(yytext, yyleng); return LONG;  }
void                    { yylval.str = new std::string(yytext, yyleng); return VOID;  }
float                   { yylval.str = new std::string(yytext, yyleng); return FLOAT; }
bool                    { yylval.str = new std::string(yytext, yyleng); return BOOL;  }
char                    { yylval.str = new std::string(yytext, yyleng); return CHAR;  }
string                  { yylval.str = new std::string(yytext, yyleng); return STR;   }
fn                      { return FUNCTION; }
extern                  { return EXTERN;   }
if                      { return IF;       }
else                    { return ELSE;     }
for                     { return FOR;      }
while                   { return WHILE;    }
let                     { return LET;      }
mut                     { return MUTABLE;  }
return                  { return RETURN;   }
break                   { return BREAK;    }
struct                  { return STRUCT;   }
template                { return TEMPLATE; }
include                 { return INCLUDE;  }
require                 { return REQUIRE;  }

[a-zA-Z_][a-zA-Z0-9_<>]*  { yylval.str = new std::string(yytext, yyleng); return IDENTIFIER; }



[[:space:]]             ; /* Ignore whitespace */
"//".*                  { /* DO NOTHING */ }
<INITIAL>"/*"           BEGIN(comment);
<comment>"*/"           BEGIN(INITIAL);
<comment>[^*\n]+        // eat comment in chunks
<comment>"*"            // eat the lone star
<comment>\n             yylineno++;
.                       { std::stringstream ss; ss << "bad input character '" << yytext << "' at line " << yylineno << std::endl; yylval.str = new std::string(ss.str()); return LEXERROR; }
%%
