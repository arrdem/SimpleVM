%{
#include <stdio.h>
#include <stdlib.h>
%}

%token address
%token number
%token ADD
%token eol

%%

statement:
    | add address address address eol
    | sub address address address eol
    | mul address address address eol
    | div address address address eol
    | abs address address address eol
    | mod address address address eol
    | set address number eol
    ;

%%

maint(int argc, char** argv) {
    yylex();
}
