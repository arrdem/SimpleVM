#ifndef _VMBLOCK_C_
#define _VMBLOCK_C_

typedef struct {
    int*     ptr;
    int      type;
    int      addr;
    int      used;
} VMBlock;

typedef struct {
    VMBlock* regs;
    int      size;
    int      used;
} VMRam;

typedef struct {
    char*    text;
    int*     code;
} VMLine;

typedef struct {
    VMRam*   memory;
    VMLine*  code;
    int      lines;
    int      cursor;
    char*    errmsg;
    int      errcode;
} VMachine;

#define VMBlockSize sizeof(VMBlock);

#endif
