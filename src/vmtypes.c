/*      vmtypes.c
 *      Copyright 2011 Reid McKenzie <rmckenzie92@gmail.com>
 *      This code and all other code in the project may be used
 *      or re-used for any purpose at all, so long as I am
 *      made aware of my contribution.
 *
 *      may be un-needed, but it lays out all the typedefs and
 *      structs used in this project just in case.
 */

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
