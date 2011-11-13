#ifndef _vmblock_c_
#define _vmblock_c_

typedef struct {
    int *ptr;
    int type;
    int addr;
    int used;
} VMBlock;

typedef struct {
    VMBlock* regs;
    int size;
    int used;
} VMRam;

#define VMBlockSize sizeof(VMBlock);

#endif
