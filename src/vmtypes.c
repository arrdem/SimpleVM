#include "stack.h"

#ifndef _VMBLOCK_H_
#define _VMBLOCK_H_

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
    VMRam*              memory;
    VMLine*             code;
    int                 lines;
    stack*              cursors;
    char*               errmsg;
    int                 errcode;
} VMachine;
#define VMBlockSize sizeof(VMBlock);

#endif
