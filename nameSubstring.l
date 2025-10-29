%{
#include <stdio.h>
%}

%%

.*suha.* { printf("Rejected: contains 'suha'\n"); }
.* { printf("Accepted: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}

int main() {
    yylex();
    return 0;
}
