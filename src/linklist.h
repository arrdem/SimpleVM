#ifndef _LINKLIST_H_
#define _LINKLIST_H_

typedef struct {
    void* next;
    void* data;
} ll;

void ll_insert(ll*, void*);
int ll_search(ll*, void*);
int ll_delete(ll*, void*);

#endif // _LINKLIST_H_
