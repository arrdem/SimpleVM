/*
 *      Copyright 2011 Reid McKenzie <rmckenzie92@gmail.com>
 *      This code and all other code in the project may be used
 *      or re-used for any purpose at all, so long as I am
 *      made aware of my contribution.
 *
 *      This file impliments the text to bytecode compiler which
 *      SimpleVM can use to decrease the size of program files.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vmtypes.h"
#include "vmachine.h"

char** USAGE = "usage: svmc <source file> <output file>\n";


char **outfile(char *data) {
    int l = strlen(data);
    int d = 1;
    while(data[l-d] != '.') d++;
    char *o = malloc(sizeof(char) * ((d > 2) ? l : l+2));
    memcpy(o, data, sizeof(char) * (l-d));
}


/**
 * ARGV[0] is the binary
 * ARGV[1] is the input file
 * ARGV[2] is the output file. Else use the input file with .VMC not .VM
*/
int main(int argc, char **argv) {
    int i = 0;
    FILE *in, *out;
    VMachine* turing;
    char* of = "a.vmc";

    if(argc < 2) {
        printf("%s", USAGE);
    } else {
        in = fopen(argv[1], "r");
        if(in) {
            turing = vm_machine_ascii(in);
            vm_machine_print(turing);

            if(argc > 2) of = &argv[2];

            out = fopen(of, "w");

            fwrite(&VMMajorVersion, 1, sizeof(int), out);
            fwrite(&VMMinorVersion, 1, sizeof(int), out);

            while(i <= turing->lines) {
                fwrite(turing->code[i].code, 1, 7*sizeof(int), out);
                i++;
            }
            fflush(out);
            fclose(out);
        } else {
            printf("ERROR - BAD FILE : %s\n",argv[i]);
        }
    }
    return 0;
}
