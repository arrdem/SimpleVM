/*
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

char **USAGE = "Usage: svm [<pcode file>] [options]\nsee man svm for options list";

int main(int argc, char **argv) {
    VMachine* turing;
    if(argc == 1){
        printf("INTERACTIVE MODE....\n");
        turing = vm_machine_ascii(stdin);

        printf("TRYING TO PRINT MACHINE...\n");
        vm_machine_print(turing);

        printf("EVALUATING.....\n");
    } else {
        printf(".VMC MODE....\n");
        turing = vm_machine_binary(fopen(argv[1], "r"));
    }

    vm_machine_run(turing);

    return 0;
}
