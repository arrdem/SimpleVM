/* this code from the o'riely book on FLEX and BISON */
%{
    enum yytokentype {
        MOD = 257,
        NUMBER = 258,
        ADD = 259,
        SUB = 260,
        MUL = 261,
        DIV = 262,
        ABS = 263,
        EOL = 264
    };

    int yyval;
%}

%%
"+"     { return ADD; }
"-"     { return SUB; }
"*"     { return MUL; }
"/"     { return DIV; }
"%"     { return MOD; }
"|"     { return ABS; }
[0-9]+  { yyval = atoi(yytext); return NUMBER; }
\n      { return EOL; }
[ \t]   {}
.       {printf("Mystery character : %c\n", *yytext); }
%%

main (int charc, char** argv) {
    int tok;
    while(tok = yylex()) {
        printf("%d", tok);
        if(tok == NUMBER) {
            printf(" = %d\n", yyval);
        } else {
            printf("\n");
        }
    }
}


