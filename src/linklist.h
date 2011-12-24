#ifndef _LINKLIST_H_
#define _LINKLIST_H_

typedef struct {
    void* next;
    void* data;
} ll;

void ll_insert(ll*, void*);
int ll_search(ll*, void*);
int ll_c_delete_node(ll*, void*);
void ll_c_delte(ll*, ll*);

#endif // _LINKLIST_H_
