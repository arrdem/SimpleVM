#include <stdio.h>
#include <stdlib.h>

#include "vmtypes.c"
#include "vconsts.c"

#ifndef _VMATH_C_
#define _VMATH_C_

VMBlock * vm_ram_malloc_dynamic(VMRam*);

void vm_math_generic(VMRam *ram, int a, int b, int c, char opr) {
    int d = 0, e = 0;
    if(ram->regs[a].used) d = *ram->regs[a].ptr;
    if(ram->regs[b].used) e = *ram->regs[b].ptr;

    while(ram->size < c) vm_ram_grow(ram);

    if(!ram->regs[c].used) {
        ram->regs[c].ptr = malloc(sizeof(int));
    }

    ram->regs[c].used = 1;
    ram->regs[c].type = VMInteger;

    switch(opr) {
        case '+':
            *ram->regs[c].ptr = d + e;
            break;

        case '-':
            *ram->regs[c].ptr = d - e;
            break;

        case '*':
            *ram->regs[c].ptr = d * e;
            break;

        case '/':
            if(e != 0) {
                *ram->regs[c].ptr = d / e;
            } else {
                printf("[MATH] ERROR - DIV/0 DETECTED. IGNORING CALCULATION.\n");
            }
            break;

        case '%':
            if(e != 0) {
                *ram->regs[c].ptr = d % e;
            } else {
                printf("[MATH] ERROR - DIV/0 DETECTED. IGNORING CALCULATION.\n");
            }
            break;

        default:
            printf("[MATH] ERROR - INVALID OPERATION SPECIFIED\n");
            break;
    }

}

void vm_math_add(VMRam *ram, int a, int b, int c) {
    vm_math_generic(ram, a, b, c, '+');
}

void vm_math_sub(VMRam *ram, int a, int b, int c) {
    vm_math_generic(ram, a, b, c, '-');
}

void vm_math_mult(VMRam *ram, int a, int b, int c) {
    vm_math_generic(ram, a, b, c, '*');
}

void vm_math_div(VMRam *ram, int a, int b, int c) {
    vm_math_generic(ram, a, b, c, '/');
}

void vm_math_mod(VMRam *ram, int a, int b, int c) {
    vm_math_generic(ram, a, b, c, '%');
}

#endif
