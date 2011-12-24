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

int main(int argc, char **argv) {
    /* ToDo List
     * - max register size
     * - initial register
     * - thread pool cap
     * - binary code
     * - text code
     * - type detect
     * - input file
     *
     *
    */

    VMachine* turing;
    if(argc < 2) turing = vm_machine_ascii(stdin);
    else         turing = vm_machine_binary(fopen(argv[1], "r"));

    printf("TRYING TO PRINT MACHINE...\n");

    vm_machine_print(turing);

    printf("EVALUATING.....\n");

    vm_machine_run(turing);

    return 0;
}
