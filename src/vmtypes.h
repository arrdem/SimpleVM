#ifndef _VMBLOCK_C_
#define _VMBLOCK_C_

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
    char* opcode;
    int  args[6];
    // basically just overkill the ram..
} VMInstr;

typedef struct {
    VMRam*   memory;
    VMInstr* code;
    int      lines;
    int      cursor;
    char*    errmsg;
    int      errcode;
} VMachine;

#define VMBlockSize sizeof(VMBlock);

#endif
