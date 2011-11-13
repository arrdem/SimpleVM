#ifndef _vmemory_c_
#define _vmemory_c_

#include <stdio.h>
#include <stdlib.h>

#include "vconsts.c"
#include "vmblock.c"
void vm_ram_display(VMRam* RAM) {
    int i = 0;
    VMBlock b;
    while(i < RAM->size) {
        b = RAM->regs[i];
        if(b.ptr != NULL) {
            printf("[%i] <used:%i type:%X data:%X, %i>\n", i, b.used, b.type, *b.ptr, *b.ptr);
        } else {
            printf("[%i] <used:%i type:%X data:NULL, NULL>\n", i, b.used);
        }
        i++;
    }
}

void vm_ram_grow(VMRam *RAM) {
    VMBlock * newREGS;
    RAM->size *= 2;
    newREGS = (VMBlock*) malloc(RAM->size * sizeof(VMBlock));

    int i = 0;
    while(i < RAM->used-1) {
        newREGS[i] = RAM->regs[i];
        i++;
    }
    free(RAM->regs);
    RAM->regs = newREGS;
}

void vm_ram_free(VMRam* RAM, int i, int j) {
    if((j)&&(RAM->regs[i].ptr != NULL)) {
        free(RAM->regs[i].ptr);
    }
    RAM->regs[i].used = VMFalse;
    RAM->regs[i].type = VMNull;
    RAM->regs[i].ptr = &VMNull;
}

VMBlock* vm_ram_malloc(VMRam* RAM) {
    if(RAM->used >= RAM->size) {
        vm_ram_grow(RAM);
    }
    return &RAM->regs[RAM->used++];
}


void vm_ram_compact(VMRam* RAM) {
    int i = 0, j = 0;
    printf("[COMPACT] ENTER                                     [OKAY]\n");
    // find first un-used cell
    while(i < RAM->size) {
        if(RAM->regs[i].used == 0) {
            break;
        } i++;
    }

    printf("[COMPACT] GOT FIRST GAP                             [OKAY]\n");

    // make both i and j the first junk index
    j = i;

    // find the next used cell, make it j
    while((RAM->regs[j].used != 1) && (i < RAM->size)) {
        j++;
    }

    // now copy the data left...
    while(j < RAM->size) {
        if(RAM->regs[j].used == 1) {
            RAM->regs[i].ptr = RAM->regs[j].ptr;
            RAM->regs[i].used = 1;
            RAM->regs[i].addr = i;
            RAM->regs[i].type = RAM->regs[j].type;
            vm_ram_free(RAM, j, 0);
            i++;
        }
        j++;
    }
}

void vm_ram_rst(VMRam *RAM) {
    RAM->used = 0;
    printf("[RST] USED                                          [OKAY]\n");
    RAM->size *= 2;
    printf("[RST] SIZE %-10i                               [OKAY]\n",RAM->size);

    if(RAM->regs) {
        free(RAM->regs);
    }
    printf("[RST] FREE                                          [OKAY]\n");

    RAM->regs = malloc(RAM->size * sizeof(VMBlock));
    printf("[RST] MALLOC(%-10i)                            [OKAY]\n",(RAM->size * sizeof(VMBlock)));

    int i = RAM->size;
    while(i >= 0) {
        RAM->regs[i].addr = i;
        RAM->regs[i].ptr  = &VMNull;
        RAM->regs[i].type = VMNull;
        RAM->regs[i].used = VMFalse;
        i--;
    }
}

#endif
