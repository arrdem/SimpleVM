#include "stack.h"

stack* stack_init() {
    return stack_init_d(4);
}

stack* stack_init_d(int d) {
    stack* s = malloc(sizeof(stack));
    s->used = 0;
    s->depth = d;
    s->data = malloc(d*sizeof(void*));
    return s;
}

int stack_depth(stack* s) {
    return s->used;
}

int stack_push(stack* s, void* d) {
    if(s->used+1 >= s->depth) {
        s->data = realloc(s->data, s->depth*2*sizeof(void*));
    }
    s->data[s->used+1] = d;
    s->used++;
}

void* stack_pop(stack* s) {
    return s->data[s->used--];
}

void* stack_peep(stack* s) {
    return s->data[s->used];
}
