#ifndef _VMEMORY_C_
#define _VMEMORY_C_

#include <stdio.h>
#include <stdlib.h>

#include "vconsts.h"
#include "vmtypes.h"

void vm_ram_display(VMRam* ram) {
    int i = 0;
    VMBlock b;
    while(i < ram->size) {
        b = ram->regs[i];
        if(b.ptr != NULL) {
            printf("[%i] <used:%i type:%X data:%X, %i>\n", i, b.used, b.type, *b.ptr, *b.ptr);
        } else {
            printf("[%i] <used:%i type:%X data:NULL, NULL>\n", i, b.used);
        }
        i++;
    }
}

int vm_ram_get(VMRam* ram, int addr) {
    if((ram->size >= addr) && ram->regs[addr].used) {
        return *ram->regs[addr].ptr;
    } else return 0;
}

void vm_ram_free(VMRam* ram, int i, int j) {
    if((j)&&(ram->regs[i].ptr != NULL)) {
        free(ram->regs[i].ptr);
    }
    ram->regs[i].used = VMFalse;
    ram->regs[i].type = VMNull;
    ram->regs[i].ptr = &VMNull;
}

void vm_ram_grow(VMRam *ram) {
    //printf("[GROW] GROWING RAM....\n");
    int i = realloc(ram->regs, ram->size * sizeof(VMBlock) * 2);
    if(i != NULL) {
        //printf("[GROW] SUCCESS.\n");
        ram->size *= 2;
    } else {
        printf("[GROW] FAILURE IS FAIL. TERMINATING.\n");
        exit(1);
    }
}

VMBlock * vm_ram_malloc_dynamic(VMRam* ram) {
    if(ram->used > ram->size - MEM_PAD) {
        vm_ram_grow(ram);
    }
    ram->regs[ram->used].used = 1;
    ram->regs[ram->used].addr = ram->used;
    return &ram->regs[ram->used++];
}

VMBlock * vm_ram_malloc_static(VMRam* ram, int index) {
    //printf("[STATIC] ALLOCATING.....\n");
    while(index > ram->size) vm_ram_grow(ram);

    if(ram->regs[index].used) free(ram->regs[index].ptr);
    ram->regs[index].ptr = malloc(sizeof(int));
    ram->regs[index].used = 1;
    ram->regs[index].addr = index;

    ram->used = index;

    return &ram->regs[index];
}

void vm_ram_compact(VMRam* ram) {
    int i = 0, j = 0;
    //printf("[COMPACT] ENTER                                     [OKAY]\n");
    // find first un-used cell
    while(i < ram->size) {
        if(ram->regs[i].used == 0) {
            break;
        } i++;
    }

    //printf("[COMPACT] GOT FIRST GAP                             [OKAY]\n");

    // make both i and j the first junk index
    j = i;

    // find the next used cell, make it j
    while((ram->regs[j].used != 1) && (i < ram->size)) {
        j++;
    }

    // now copy the data left...
    while(j < ram->size) {
        if(ram->regs[j].used == 1) {
            ram->regs[i].ptr = ram->regs[j].ptr;
            ram->regs[i].used = 1;
            ram->regs[i].addr = i;
            ram->regs[i].type = ram->regs[j].type;
            vm_ram_free(ram, j, 0);
            i++;
        }
        j++;
    }
}

void vm_ram_rst(VMRam *ram) {
    ram->used = 0;
    //printf("[RST] USED                                          [OKAY]\n");

    if(ram->regs) {
        free(ram->regs);
    }
    //printf("[RST] FREE                                          [OKAY]\n");

    ram->regs = malloc(ram->size * sizeof(VMBlock));
    //printf("[RST] MALLOC(%-10i)                            [OKAY]\n",(ram->size));

    int i = ram->size;
    while(i >= 0) {
        ram->regs[i].addr = i;
        ram->regs[i].ptr  = &VMNull;
        ram->regs[i].type = VMNull;
        ram->regs[i].used = VMFalse;
        i--;
    }
}

void vm_ram_assign_static(VMRam *ram, int index, int value) {
    //printf("[STATIC] ASSIGNING.... \n");
    while(ram->size < index) vm_ram_grow(ram);


    if(ram->regs[index].used) free(ram->regs[index].ptr);
    ram->regs[index].ptr = malloc(sizeof(int));
    *ram->regs[index].ptr = value;
    ram->regs[index].used = 1;
    ram->regs[index].type = VMInteger;
}

void vm_ram_assign_dynamic(VMRam *ram, int value) {
    ram->used++;

    if(ram->used > ram->size - MEM_PAD) {
        if(ram->regs[ram->used].used) {
            free(ram->regs[ram->used].ptr);
        }

        ram->regs[ram->used].ptr = malloc(sizeof(int));
        *ram->regs[ram->used].ptr = value;
        ram->regs[ram->used].used = 1;
        ram->regs[ram->used].type = VMInteger;

    } else {
        printf("[ASSIGN] WARNING - ALLOCATED TO BLOCK %-10i    [OKAY]\n", ram->used);
        VMBlock * a = vm_ram_malloc_dynamic(ram);
        a->ptr = malloc(sizeof(int));
        *a->ptr = value;
        a->used = 1;
        a->type = VMInteger;
    }
}

VMRam * vm_ram_init() {
    VMRam * ram;
    ram = malloc(sizeof(VMRam));
    ram->regs = malloc(sizeof(VMBlock) * MIN_MEM);
    memset(ram->regs, 0, sizeof(VMBlock) * MIN_MEM);
    ram->size = MIN_MEM;
    return ram;
}

#endif
