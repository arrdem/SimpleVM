#include <stdlib.h>

typedef struct {
    int depth;
    int used;
    int *stack;
} stack;

stack* stack_init(int depth) {
    stack* s = malloc(sizeof(stack));
    s->depth = depth;
    s->stack = malloc(sizeof(int)*depth);
    s->used = 0;
    return s;
}

void stack_push(stack* s, int e) {
    if(s->depth < (s->used + 1)) {
       // resize the array
       realloc(s->stack, sizeof(int) * s->depth * 2);
       s->depth *= 2;
    }
    s->stack[s->used] = e;
    s->used++;
}

int stack_pop(stack* s) {
    return s->stack[(s->used--) - 1];
}

int stack_peep(stack* s) {
    return s->stack[s->used];
}

int stack_depth(stack* s) {
    return s->used;
}

int stack_empty(stack* s) {
    return !(s->used > 0);
}
