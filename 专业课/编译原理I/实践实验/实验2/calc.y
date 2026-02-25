%{
#include <stdio.h>
#include <string.h>
int yylex(void);
void yyerror(char *);
%}

%union{
  int inum;
  double dnum;
}
%token ADD SUB MUL DIV CR
%token <inum> NUM
%type  <inum> expression term single

%%
       line_list: line
                | line_list line
                ;
				
	       line : expression CR  {printf(">>%d\n",$1);}

      expression: term 
                | expression ADD term   {$$=$1+$3;}
				| expression SUB term   {$$=$1-$3;}
                ;

            term: single
				| term MUL single		{$$=$1*$3;}
				| term DIV single		{$$=$1/$3;}
				;
				
		  single: NUM
				;
%%
void yyerror(char *str){
    fprintf(stderr,"error:%s\n",str);
}

int yywrap(){
    return 1;
}
int main()
{
    yyparse();
}
