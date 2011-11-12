#ifndef _vmemory_c_
#define _vmemory_c_

typedef struct {
    int *ptr;
    int type;
    int addr;
    int used;
} VMBlock;

#define VMBlockSize sizeof(VMBlock);

#endif
