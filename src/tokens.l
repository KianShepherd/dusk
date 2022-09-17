%{
#include <string>
#include <cstdio>
#include <sstream>
#include "parser.h"
/*

%token<str> IDENTIFIER
*/
%}

%option noinput nounput noyywrap nodefault
%option yylineno
%option outfile="src/scanner.cc"
%%
[0-9]+                  { yylval.str = new std::string(yytext, yyleng); return NUMBER;  }
[0-9]+\.[0-9]+          { yylval.str = new std::string(yytext, yyleng); return DOUBLE;  }
\"([^\\\"]|\\.)*\"      { yylval.str = new std::string(yytext, yyleng); return STRING;  }
True                    { yylval.str = new std::string(yytext, yyleng); return TRUE;    }
False                   { yylval.str = new std::string(yytext, yyleng); return FALSE;   }
Null                    { yylval.str = new std::string(yytext, yyleng); return NULLTOK; }

\+                      { return PLUS;       }
\-                      { return MINUS;      }
\*                      { return TIMES;      }
\/                      { return DIVIDE;     }
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
{                       { return RBRACE;    }
}                       { return LBRACE;    }
\(                      { return RPAREN;    }
\)                      { return LPAREN;    }
:                       { return COLON;     }
,                       { return COMMA;     }
->                      { return ARROW;     }

int                     { return INT;      }
void                    { return VOID;     }
float                   { return FLOAT;    }
bool                    { return BOOL;     }
fn                      { return FUNCTION; }
if                      { return IF;       }
else                    { return ELSE;     }
for                     { return FOR;      }
while                   { return WHILE;    }
let                     { return LET;      }
mut                     { return MUTABLE;  }
return                  { return RETURN;   }

^[a-zA-Z][a-zA-Z0-9]*   { yylval.str = new std::string(yytext, yyleng); return IDENTIFIER; }

[[:space:]]             ; /* Ignore whitespace */
.                       { std::stringstream ss; ss << "bad input character '" << yytext << "' at line " << yylineno << std::endl; yylval.str = new std::string(ss.str()); return LEXERROR; }
%%
