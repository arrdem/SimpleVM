/*
 *      Copyright 2011 Reid McKenzie <rmckenzie92@gmail.com>
 *      This code and all other code in the project may be used
 *      or re-used for any purpose at all, so long as I am
 *      made aware of my contribution.
 */

#include <stdio.h>
#include <stdlib.h>

#include "vmtypes.h"
#include "vmachine.h"

int main(int argc, char **argv) {
    VMachine* turing;
    turing = vm_machine(stdin);

    printf("TRYING TO PRINT MACHINE...\n");

    vm_machine_print(turing);

    printf("EVALUATING.....\n");

    vm_machine_run(turing);

    return 0;
}
