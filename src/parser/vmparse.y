/* this code from the o'riely book on FLEX and BISON */
%{
#include <stdio.h>
%}

%token NUMBER
%token ADD SUB MUL DIV
%token EOL

%%

calclist:
    | calclist exp EOL { printf("= %i\n", $1); }
    ;

exp: factor { $$ = $1;}
    | exp ADD factor { printf("ADDING %i, %i\n", $1, $3); $$ = $1 + $3; }
    | exp SUB factor { $$ = $1 - $3; }
    ;

factor: term { $$ = $1;}
    | factor MUL term { $$ = $1 * $3; }
    | factor DIV term { $$ = $1 / $3; }
    ;

term: NUMBER { $$ = $1;}
    ;
%%

main(int argc, char** argv)
{
    yyparse();
}

yyerror(char* s)
{
    fprintf(stderr, "error %s\n", s);
}
