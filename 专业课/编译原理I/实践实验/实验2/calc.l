%{
#include <stdio.h>
#include "y.tab.h"
void yyerror(char *);
%}
NUM [1-9]+[0-9]*|0
%%

{NUM}		                sscanf(yytext,"%d",&yylval.inum); return NUM;
"+"							return ADD;
"-"							return SUB;
"*"							return MUL;
"/"							return DIV;
\n                          return CR;
[ \t]+                     /* ignore whitespace */;
.
%%
