#ifndef _vmemory_c_
#define _vmemory_c_

#include <stdio.h>
#include <stdlib.h>

#include "vconsts.c"
#include "vmtypes.c"

VMRam* vm_ram_init();

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

void vm_ram_free(VMRam* ram, int i, int j) {
    if((j)&&(ram->regs[i].ptr != NULL)) {
        free(ram->regs[i].ptr);
    }
    ram->regs[i].used = VMFalse;
    ram->regs[i].type = VMNull;
    ram->regs[i].ptr = &VMNull;
}

void vm_ram_grow(VMRam *ram) {
    VMBlock * newREGS;
    ram->seg_count += ram->size;
    newREGS = malloc(2 * ram->size * sizeof(VMBlock));

    int i = 0;
    while(i < ram->size) {
        if(ram->regs[i].used) {
            newREGS[i] = ram->regs[i];
            newREGS[i].ptr = malloc(sizeof(int));
            *newREGS[i].ptr = *ram->regs[i].ptr;
            vm_ram_free(ram, i, 1);
        }
        i++;

    }
    free(ram->regs);
    ram->regs = newREGS;
    ram->size *= 2;
}

VMBlock * vm_ram_malloc(VMRam* ram, int size) {
    if(ram->used + size > ram->size - MEM_PAD) {
        vm_ram_grow(ram);
    }
    int i = ram->seg_count, j = 0;
    while(i >= 0) {
        if(ram->segments[i].length >= size) {
            break;
        }
        i--;
    }

    VMBlock *b;
    b = ram->segments[i].b;
    ram->segments[i].length -= size;
    ram->segments[i].b += size;

    // set used flags
    while(j < size) {
        *(b + j)->used = 1;
    }

    vm_ram_seg_clean(ram);

    return b;
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
    while(ram->seg_count-- > 0) { // possible off by 1
        free(ram->segments[ram->seg_count].b);
    }
    free(ram->segments);

    while(ram->size-- > 0) { // probable segfault due to free()int null
        if(*ram->regs[ram->size].ptr) {
            free(ram->regs[ram->size].ptr);
        }
    }
    free(ram->regs);

    ram = vm_ram_init();
}

void vm_ram_assign(VMRam *ram, int index, int value) {
    if(ram->regs[index].used) {
        *ram->regs[index].ptr = value;
    } else {
        VMBlock * a = vm_ram_malloc(ram);
        printf("[ASSIGN] WARNING - ALLOCATED TO BLOCK %-10i    [OKAY]\n", a->addr);
        a->ptr = malloc(sizeof(int));
        *a->ptr = value;
    }
}

VMRam * vm_ram_init() {
    VMRam * ram;
    ram = malloc(sizeof(VMRam));
    ram->regs = malloc(sizeof(VMBlock));
    ram->size = MIN_MEM;

    // auto init all vars to null
    int i = ram->size;
    while(i >= 0) {
        ram->regs[i].addr = i;
        ram->regs[i].ptr  = &VMNull;
        ram->regs[i].type = VMNull;
        ram->regs[i].used = VMFalse;
        i--;
    }

    // new code for RAM segmentation...
    ram->segments  = malloc(MIN_SEGS * sizeof(VMSegment));
    ram->seg_count = 1;
    ram->seg_size  = MIN_SEGS;

    ram->segments[0].b = ram->regs;
    ram->segments[0].length = ram->size;
    return ram;
}

#endif
