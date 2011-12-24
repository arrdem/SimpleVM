/*      vmachine.h
 *      Copyright 2011 Reid McKenzie <rmckenzie92@gmail.com>
 *      This code and all other code in the project may be used
 *      or re-used for any purpose at all, so long as I am
 *      made aware of my contribution.
 *
 *      Defines the loader code for the VMachine struct, and also the
 *      VMachine-based execution functions which actually perform
 *      VM instruction execution.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "vmemory.h"
#include "vconsts.h"

long vm_machine_string_hash(char*);
void vm_machine_print(VMachine*);
VMachine* vm_machine_ascii(FILE*);
VMachine* vm_machine_binary(FILE*);
void vm_machine_eval(VMachine*);
void vm_machine_run(VMachine*);

