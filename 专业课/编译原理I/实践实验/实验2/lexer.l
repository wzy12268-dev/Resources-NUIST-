%{
	#include "y.tab.h"
	void yyerror(char *);
%}

INUM	[1-9]+[0-9]*|0
FNUM	[0-9]+\.[0-9]+
VAR 	[a-zA-Z_$]+[a-zA-Z_$0-9]*
CHAR	'.'

%%

"int"		return DEF_INT;
"double"	return DEF_DOUBLE;
"char"		return DEF_CHAR;
{INUM}		return INT;
{FNUM}		return DOUBLE;
{VAR}		return VAR;
{CHAR}		return CHAR;
"="			return EQ;
";"			return END;
"\n"		return CR;
%%
