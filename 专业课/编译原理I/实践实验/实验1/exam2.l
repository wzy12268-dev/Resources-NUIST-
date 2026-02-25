%{
#include <stdio.h>

#define WHILE       18
#define DO          19
#define ID          20
#define NUMBER      21
#define RELOP       22
#define NEWLINE     23
#define ERRORCHAR   24
#define IF          11
#define ELSE        12
%}

delim   [ \t\r\n]
ws      {delim}+
letter  [A-Za-z_]
digit   [0-9]
id      {letter}({letter}|{digit})*
hexnum  0[xX][0-9A-Fa-f]+
number  {digit}+(\.{digit}+)?([eE][+-]?{digit}+)?
str     \"([^\\\n]|\\.)*\"

%s COMMENT
%s COMMENT2

%%
<INITIAL>"/*"            { BEGIN COMMENT; }
<COMMENT>"*/"            { BEGIN INITIAL; }
<COMMENT>.|\n            { ; }

<INITIAL>"//"            { BEGIN COMMENT2; }
<COMMENT2>\n             { BEGIN INITIAL; }
<COMMENT2>.              { ; }

<INITIAL>{ws}            { ; }

<INITIAL>{str}           { return ID; }

<INITIAL>"#"             { return RELOP; }
<INITIAL>"."             { return RELOP; }
<INITIAL>","             { return RELOP; }
<INITIAL>"["             { return RELOP; }
<INITIAL>"]"             { return RELOP; }

<INITIAL>while           { return WHILE; }
<INITIAL>do              { return DO; }
<INITIAL>if              { return IF; }
<INITIAL>else            { return ELSE; }

<INITIAL>{id}            { return ID; }
<INITIAL>{hexnum}        { return NUMBER; }
<INITIAL>{number}        { return NUMBER; }

<INITIAL>"<="            { return RELOP; }
<INITIAL>">="            { return RELOP; }
<INITIAL>"=="            { return RELOP; }
<INITIAL>"!="            { return RELOP; }
<INITIAL>"<>"            { return RELOP; }
<INITIAL>"<"             { return RELOP; }
<INITIAL>">"             { return RELOP; }
<INITIAL>"="             { return RELOP; }

<INITIAL>"("             { return RELOP; }
<INITIAL>")"             { return RELOP; }
<INITIAL>"{"             { return RELOP; }
<INITIAL>"}"             { return RELOP; }
<INITIAL>"+"             { return RELOP; }
<INITIAL>"-"             { return RELOP; }
<INITIAL>"*"             { return RELOP; }
<INITIAL>"/"             { return RELOP; }
<INITIAL>";"             { return RELOP; }

<INITIAL>.               { return ERRORCHAR; }
%%

int yywrap() { return 1; }

void writeout(int c){
  switch(c){
    case ERRORCHAR: fprintf(yyout, "(ERRORCHAR, \"%s\") ", yytext); break;
    case RELOP:     fprintf(yyout, "(RELOP, \"%s\") ", yytext); break;
    case WHILE:     fprintf(yyout, "(WHILE, \"%s\") ", yytext); break;
    case DO:        fprintf(yyout, "(DO, \"%s\") ", yytext); break;
    case IF:        fprintf(yyout, "(IF, \"%s\") ", yytext); break;
    case ELSE:      fprintf(yyout, "(ELSE, \"%s\") ", yytext); break;
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
    if (argc>=3){
      yyout = fopen(argv[2], "w");
    }
  }

  while ((c = yylex())){
    writeout(c);
    j++;
    if (j%5 == 0) writeout(NEWLINE);
  }

  if(argc>=2){
    fclose(yyin);
    if (argc>=3) fclose(yyout);
  }
  return 0;
}
