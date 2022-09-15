%{
#include <stdio.h>
#include <string.h>

extern "C" {
    int yyparse(void);

    int yylex(void);  

    int yywrap() {
            return 1;
    }

    void yyerror(const char *str) {
	    fprintf(stderr,"error: %s\n",str);
    }
}


int main()
{
	yyparse();
    return 0;
}
%}

%token NUMBER TOKHEAT STATE TOKTARGET TOKTEMPERATURE

%%

commands: /* empty */
	| commands command
	;


command:
	heat_switch
	|
	target_set
	;

heat_switch:
	TOKHEAT STATE 
	{
		printf("\tHeat turned on or off\n");
	}
	;

target_set:
	TOKTARGET TOKTEMPERATURE NUMBER
	{
		printf("\tTemperature set\n");
	}
	;
