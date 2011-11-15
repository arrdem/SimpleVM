#ifndef _vmblock_c_
#define _vmblock_c_

typedef struct {
    int *ptr;
    int type;
    int addr;
    int used;
} VMBlock;

typedef struct {
    VMBlock* b;
    int length;
} VMSegment;

typedef struct {
    VMSegment* segments;
    int seg_count;
    int seg_size;
    VMBlock* regs;
    int size;
} VMRam;

typedef struct {
    char **strings;
    int count;
} split;

typedef struct {
    char *msg;
    int code;
} error;

typedef struct {
    char **code;
    int lines;
    int eval;
    VMRam* state;
} VMState;

#define VMBlockSize sizeof(VMBlock);

#endif
