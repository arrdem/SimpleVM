#include <stdio.h>
#include <stdlib.h>

#include "vmtypes.c"

#ifndef _vmath_c_
#define _vmath_c_

VMBlock * vm_ram_malloc_dynamic(VMRam*);

void vm_math_generic(VMRam *ram, int a, int b, int c, char opr) {
    int d = 0, e = 0, f = c;
    if(ram->regs[a].used) d = *ram->regs[a].ptr;
    if(ram->regs[b].used) e = *ram->regs[b].ptr;
    if(!ram->regs[c].used) {
        VMBlock * foo = vm_ram_malloc_dynamic(ram);
        f = foo->addr;
        foo->ptr = malloc(sizeof(int));
        printf("[MATH] WARNING - %-10i WAS NOT INITIALIZED, USING %i\n", c, f);
    }
    switch(opr) {
        case '+':
            *ram->regs[f].ptr = d + e;
            break;

        case '-':
            *ram->regs[f].ptr = d - e;
            break;

        case '*':
            *ram->regs[f].ptr = d * e;
            break;

        case '/':
            if(e != 0) {
                *ram->regs[f].ptr = d / e;
            } else {
                printf("[MATH] ERROR - DIV/0 DETECTED. IGNORING CALCULATION.\n");
            }
            break;

        case '%':
            if(e != 0) {
                *ram->regs[f].ptr = d % e;
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
