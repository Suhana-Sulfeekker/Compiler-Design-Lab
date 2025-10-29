%{
#include "calc.tab.h"
#include <stdio.h>
%}

%option noyywrap

%%

[0-9]+      { yylval = atoi(yytext); return NUMBER; }
[ \t]       ; 
\n          { return '\n'; }
"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MUL; }
"/"         { return DIV; }
"("         { return LPAREN; }
")"         { return RPAREN; }

.           { printf("Invalid character: %s\n", yytext); }

%%

