%{
#include <stdio.h>

#define LT          1
#define LE          2
#define GT          3
#define GE          4
#define EQ          5
#define NE          6

#define WHILE       18
#define DO          19
#define ID          20
#define NUMBER      21
#define RELOP       22

#define NEWLINE     23
#define ERRORCHAR   24

int yylval;
%}

/* definitions */
delim       [ \t\n]
ws          {delim}+
letter      [A-Za-z_]
digit       [0-9]
id          {letter}({letter}|{digit})*
number      {digit}+(\.{digit}+)?(E[+-]?{digit}+)?

%%
  /* skip whitespace */
{ws}            { ; }

  /* keywords */
while           { return WHILE; }
do              { return DO; }

  /* id / number */
{id}            { yylval = installID();  return ID; }
{number}        { yylval = installNum(); return NUMBER; }

  /* relops: LONGER FIRST */
"<="            { yylval = LE; return RELOP; }
">="            { yylval = GE; return RELOP; }
"<>"            { yylval = NE; return RELOP; }
"<"             { yylval = LT; return RELOP; }
">"             { yylval = GT; return RELOP; }
"="             { yylval = EQ; return RELOP; }

  /* extra symbols (to avoid ERRORCHAR) */
"+"             { yylval = 0;  return RELOP; }
"-"             { yylval = 0;  return RELOP; }
"*"             { yylval = 0;  return RELOP; }
"/"             { yylval = 0;  return RELOP; }
"("             { yylval = 0;  return RELOP; }
")"             { yylval = 0;  return RELOP; }
";"             { yylval = 0;  return RELOP; }

  /* fallback */
.               { yylval = ERRORCHAR; return ERRORCHAR; }
%%

int installID()  { return ID; }
int installNum() { return NUMBER; }

int yywrap() { return 1; }

void writeout(int c){
  switch(c){
    case ERRORCHAR: fprintf(yyout, "(ERRORCHAR, \"%s\") ", yytext); break;
    case RELOP:     fprintf(yyout, "(RELOP, \"%s\") ", yytext); break;
    case WHILE:     fprintf(yyout, "(WHILE, \"%s\") ", yytext); break;
    case DO:        fprintf(yyout, "(DO, \"%s\") ", yytext); break;
    case NUMBER:    fprintf(yyout, "(NUM, \"%s\") ", yytext); break;
    case ID:        fprintf(yyout, "(ID, \"%s\") ", yytext); break;
    case NEWLINE:   fprintf(yyout, "\n"); break;
    default: break;
  }
}

int main (int argc, char ** argv){
  int c, j=0;

  if (argc>=2){
    if ((yyin = fopen(argv[1], "r")) == NULL){
      printf("Can't open file %s\n", argv[1]);
      return 1;
    }
    if (argc>=3) yyout = fopen(argv[2], "w");
  }

  while ((c = yylex())){
    writeout(c);
    j++;
    if (j%5 == 0) writeout(NEWLINE);
  }

  if (argc>=2){
    fclose(yyin);
    if (argc>=3) fclose(yyout);
  }
  return 0;
}
