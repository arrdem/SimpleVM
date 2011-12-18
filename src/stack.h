#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

typedef struct {
    int depth;
    int used;
    int *stack;
} stack;

stack* stack_init(int);
void stack_push(stack*, int);
int stack_pop(stack*);
int stack_peep(stack*);
int  stack_depth(stack*);
int  stack_empty(stack*);

#endif // STACK_H_INCLUDED
