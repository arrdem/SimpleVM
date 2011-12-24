#include "linklist.h"
#include <stdlib.h>

#ifndef _LINKLIST_C_
#define _LINKLIST_C_

void ll_insert(ll* c, void* d) {
    ll* n = malloc(sizeof(ll));
    n->next = c->next;
    n->data = d;
    c->next = n;
}

int ll_search(ll* o, void* d) {
    int i = 0;
    ll* c;
    c = o;
    while((c != o) && (c->data != d) && (c->next)) {
        c = c->next;
        i++;
    }
    if(c->data == d) return i;
    else return -1;
}

int ll_c_delete_node(ll* o, void* d) {
    ll *c, *t;
    t = o;
    c = t->next;
    while((c != o) && (c->next) && (c->data != d)) {
        t = c;
        c = c->next;
    }
    if(c->data == d) {
        t->next = c->next;
        free(c);
        return 1;
    }
    return -1;
}

void ll_c_delete(ll* n, ll* s) {
    free(n->data);
    if((n->next) && (n->next != s)) ll_c_delete(n->next, s);
    free(n->next);
}

#endif
