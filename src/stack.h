/*      stack.h
 *      Copyright 2011 Reid McKenzie <rmckenzie92@gmail.com>
 *      This code and all other code in the project may be used
 *      or re-used for any purpose at all, so long as I am
 *      made aware of my contribution.
 *
 *      qick-hack stack of integers.
 */

#include "vmtypes.h"

#ifndef _STACK_H_
#define _STACK_H_

typedef struct {
    int depth;
    int used;
    void **stack;
} stack;

stack* stack_init(int);
void stack_push(stack*, void*);
void* stack_pop(stack*);
void* stack_peep(stack*);
int  stack_depth(stack*);
int  stack_empty(stack*);

#endif // _STACK_H_
