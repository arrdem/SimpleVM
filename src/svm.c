/**
 *      Copyright 2011 Reid McKenzie <rmckenzie92@gmail.com>
 *      This code and all other code in the project may be used
 *      or re-used for any purpose at all, so long as I am
 *      made aware of my contribution.
 *
 *      This file impliments the standard SimpleVM console
 *      environment.
*/

#include <stdio.h>
#include <stdlib.h>

#include "vmtypes.h"
#include "vmachine.h"

char **USAGE = "Usage: svm [--ascii <code file>] [<pcode file>]\nsee man svm for options list";

int main(int argc, char **argv) {
    VMachine* turing;

    if((argc >= 2) && (strcmp("-?", argv[1]) == 0)) {
        // print the USAGE info
        printf("%s\n", USAGE);
    }
    else if ((argc >= 3) && (strcmp("--ascii", argv[1]) == 0)) {
        // use a file, ASCII code
        turing = vm_machine_ascii(fopen(argv[2], "r"));
    }
    else if (argc > 1) {
        // use a file, assume a precompiled file
        printf(".VMC MODE....\n");
        turing = vm_machine_binary(fopen(argv[1], "r"));
    }
    else {
        // no file, so assume interactive mode
        printf("INTERACTIVE MODE....\n");
        turing = vm_machine_ascii(stdin);

        printf("TRYING TO PRINT MACHINE...\n");
        vm_machine_print(turing);

        printf("EVALUATING.....\n");
    }

    vm_machine_run(turing);

    return 0;
}
