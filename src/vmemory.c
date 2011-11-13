#ifndef _vmemory_c_
#define _vmemory_c_

#include <stdio.h>
#include <stdlib.h>

#include "vconsts.c"
#include "vmblock.c"

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

void vm_ram_grow(VMRam *ram) {
    VMBlock * newREGS;
    ram->size *= 2;
    newREGS = (VMBlock*) malloc(ram->size * sizeof(VMBlock));

    int i = 0;
    while(i < ram->used-1) {
        newREGS[i] = ram->regs[i];
        i++;
    }
    free(ram->regs);
    ram->regs = newREGS;
}

void vm_ram_free(VMRam* ram, int i, int j) {
    if((j)&&(ram->regs[i].ptr != NULL)) {
        free(ram->regs[i].ptr);
    }
    ram->regs[i].used = VMFalse;
    ram->regs[i].type = VMNull;
    ram->regs[i].ptr = &VMNull;
}

VMBlock* vm_ram_malloc(VMRam* ram) {
    if(ram->used >= ram->size) {
        vm_ram_grow(ram);
    }
    return &ram->regs[ram->used++];
}


void vm_ram_compact(VMRam* ram) {
    int i = 0, j = 0;
    printf("[COMPACT] ENTER                                     [OKAY]\n");
    // find first un-used cell
    while(i < ram->size) {
        if(ram->regs[i].used == 0) {
            break;
        } i++;
    }

    printf("[COMPACT] GOT FIRST GAP                             [OKAY]\n");

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
    printf("[RST] USED                                          [OKAY]\n");

    if(ram->regs) {
        free(ram->regs);
    }
    printf("[RST] FREE                                          [OKAY]\n");

    ram->regs = malloc(ram->size * sizeof(VMBlock));
    printf("[RST] MALLOC(%-10i)                            [OKAY]\n",(ram->size));

    int i = ram->size;
    printf("%i\n", i);
    goto die;
    while(i >= 0) {
        printf("%i\n", i);
        ram->regs[i].addr = i;
        ram->regs[i].ptr  = &VMNull;
        ram->regs[i].type = VMNull;
        ram->regs[i].used = VMFalse;
        i--;
    }
    die:
    printf("DIED\n");
}

void vm_ram_assign(VMRam *ram, int index, int value) {
    if(index < ram->used) {
        *ram->regs[index].ptr = value;
    }
}

VMRam * vm_ram_init() {
    VMRam * ram;
    ram = malloc(sizeof(VMRam));
    ram->regs = malloc(sizeof(VMBlock));
    ram->size = 5;
    return ram;
}


#endif
