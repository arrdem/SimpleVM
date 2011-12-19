/*      stack.c
 *      Copyright 2011 Reid McKenzie <rmckenzie92@gmail.com>
 *      This code and all other code in the project may be used
 *      or re-used for any purpose at all, so long as I am
 *      made aware of my contribution.
 *
 *      qick-hack stack of integers.
 */

#include <stdlib.h>
#include "vmtypes.h"
#include "stack.h"

#ifndef STACK_C_
#define STACK_C_

stack* stack_init(int depth) {
    stack* s = malloc(sizeof(stack));
    s->depth = depth;
    s->stack = malloc(sizeof(void*)*depth);
    s->used = 0;
    return s;
}

void stack_push(stack* s, void* e) {
    if(s->depth < (s->used + 1)) {
       // resize the array
       realloc(s->stack, sizeof(int) * s->depth * 2);
       s->depth *= 2;
    }
    s->stack[s->used] = e;
    s->used++;
}

void* stack_pop(stack* s) {
    return s->stack[(s->used--) - 1];
}

void* stack_peep(stack* s) {
    return s->stack[s->used];
}

int stack_depth(stack* s) {
    return s->used;
}

int stack_empty(stack* s) {
    return !(s->used > 0);
}

#endif
