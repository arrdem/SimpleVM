/* this code from the o'riely book on FLEX and BISON */
%{
#include "vmparse.tab.h"
int yyval;
%}

%%
"+"     { return ADD; }
"-"     { return SUB; }
"*"     { return MUL; }
"/"     { return DIV; }
[0-9]+  { yyval = atoi(yytext); printf("INT %i; ", yyval); return NUMBER; }
\n      { return EOL; }
[ \t]   {}
.       { printf("Mystery character : %c\n", *yytext); }
%%
