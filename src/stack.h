#ifndef STACK_H
#define STACK_H

typedef struct stack stack;

struct stack {
    int depth, used;
    void** data;
};

stack *stack_init();
stack *stack_init_d(int);
int stack_depth(stack*);
int stack_push(stack*, void*);
void* stack_pop(stack*);
void* stack_peep(stack*);

#endif // STACK_H
