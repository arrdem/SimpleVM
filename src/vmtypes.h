/*      vmtypes.h
 *      Copyright 2011 Reid McKenzie <rmckenzie92@gmail.com>
 *      This code and all other code in the project may be used
 *      or re-used for any purpose at all, so long as I am
 *      made aware of my contribution.
 *
 *      lays out all the types and structs nicely for GCC
 */

#include "linklist.h"

#ifndef _VMBLOCK_H_
#define _VMBLOCK_H_

typedef struct {
    int      ptr;
    unsigned int meta;
    /**
     * meta is 32 bits of unsigned goodness.
     * bit 0    - block used (has it been assigned to ever) (zeroed on free())
     * bit 1    - block lock (is it locked to pointer only)
     * bit 2    - block is value an int
     * bit 3    - block is value a pointer type
     *    if(![2] && ![3]) then ![0] and the block should be untyped
    */
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
    int      line;
    int      id;
} VMThread;

typedef struct {
    // stuff from 1.0 for the machine
    VMRam*   memory;
    VMLine*  code;
    int      lines;

    ll*      threads;
    int      threadcount;

    char*    errmsg;
    int      errcode;
} VMachine;

#define VMBlockSize sizeof(VMBlock);

#endif
