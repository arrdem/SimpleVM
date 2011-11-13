#include <stdio.h>
#include <stdlib.h>

#include "vmblock.c"

void vm_add(VMBlock *a, VMBlock  *b, VMBlock  *c) {
    *c->ptr = (*a->ptr + *b->ptr);
}

void vm_sub(VMBlock *a, VMBlock  *b, VMBlock  *c) {
    *c->ptr = (*a->ptr - *b->ptr);
}

void vm_mult(VMBlock *a, VMBlock  *b, VMBlock  *c) {
    *c->ptr = ((*a->ptr)*(*b->ptr));
}

void vm_div(VMBlock *a, VMBlock *b, VMBlock *c) {
    *c->ptr = ((*a->ptr)/(*b->ptr));
}
