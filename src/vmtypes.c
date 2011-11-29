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

typedef struct {
    VMRam* memory;
    char** code;
    int    lines;
    int    cursor;
    char*  errmsg;
    int    errcode;
} VMachine;

typedef struct {
    char **strings;
    int count;
} split;

#define VMBlockSize sizeof(VMBlock);

#endif
