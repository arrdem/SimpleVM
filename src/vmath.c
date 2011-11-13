#include <stdio.h>
#include <stdlib.h>

#include "vmblock.c"

VMBlock * vm_ram_malloc(VMRam*);

void vm_math_generic(VMRam *ram, int a, int b, int c, char opr) {
    int d = 0, e = 0, f = c;
    if(ram->regs[a].used) d = *ram->regs[a].ptr;
    if(ram->regs[b].used) e = *ram->regs[b].ptr;
    if(!ram->regs[c].used) {
        VMBlock * foo = vm_ram_malloc(ram);
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
            *ram->regs[f].ptr = d / e;
            break;
        default:
            printf("[MATH] ERROR - INVALID OPERATION SPECIFIED\N");
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
