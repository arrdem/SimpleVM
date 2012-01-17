/*      vmemory.c
 *      Copyright 2011 Reid McKenzie <rmckenzie92@gmail.com>
 *      This code and all other code in the project may be used
 *      or re-used for any purpose at all, so long as I am
 *      made aware of my contribution.
 *
 *      impliments the VM's memory system
 */

#ifndef _VMEMORY_C_
#define _VMEMORY_C_

#include <stdio.h>
#include <stdlib.h>

#include "vconsts.h"
#include "vmtypes.h"
#include "vmemory.h"

/*==============================================================
        ACTIVE CODE
==============================================================*/
void vm_ram_display(VMRam* ram) {
    int i = 0;
    VMBlock b;
    while(i < ram->size) {
        b = ram->regs[i];
        printf("[%-3i] used:%-5i type:%-8X data:%-8X %-10i\n",
               i++,
               vm_ram_addrmeta_used(ram, i),
               vm_ram_addrmeta_type(ram, i),
               b.ptr, b.ptr);
    }
}

int vm_ram_get(VMRam* ram, int addr) {
    if((ram->size >= addr) && vm_ram_addrmeta_used(ram, addr)) {
        return ram->regs[addr].ptr;
    } else return 0;
}

void vm_ram_free(VMRam* ram, int i, int j) {
    vm_ram_addrmeta_used_set(ram, i, 0);
    vm_ram_addrmeta_type_set(ram, i, 0);
    ram->regs[i].ptr =  VMNull;
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
        ram->regs[i].ptr  = &VMNull;
        vm_ram_addrmeta_type_set(ram, i, 0);
        vm_ram_addrmeta_used_set(ram, i, 0);
        i--;
    }
}

void vm_ram_assign_static(VMRam *ram, int index, int value) {
    //printf("[STATIC] ASSIGNING.... \n");
    while(ram->size < index) vm_ram_grow(ram);
    if(!vm_ram_addrmeta_used(ram, index)) vm_ram_addrmeta_used_set(ram, index, 1);
    ram->regs[index].ptr = value;
    vm_ram_addrmeta_type_set(ram, index, 1);
}

VMRam * vm_ram_init() {
    VMRam * ram;
    ram = malloc(sizeof(VMRam));
    ram->regs = malloc(sizeof(VMBlock) * MIN_MEM);
    memset(ram->regs, 0, sizeof(VMBlock) * MIN_MEM);
    ram->size = MIN_MEM;
    return ram;
}

void vm_ram_addrmeta_type_set(VMRam *ram, int addr, int val) {
    ram->regs[addr].meta |= (val << 3);
}

void vm_ram_addrmeta_used_set(VMRam *ram, int addr, int val) {
    ram->regs[addr].meta |= (val << 0);
}

void vm_ram_addrmeta_heap_set(VMRam *ram, int addr, int val) {
    ram->regs[addr].meta |= (val << 4);
}

int vm_ram_addrmeta_type(VMRam *ram, int addr) {
    return (ram->regs[addr].meta & (3 << 2));
}

int vm_ram_addrmeta_used(VMRam *ram, int addr) {
    return (ram->regs[addr].meta & (1 << 0));
}

int vm_ram_addrmeta_heap(VMRam *ram, int addr) {
    return (ram->regs[addr].meta & (1 << 4));
}

/*==============================================================
        DEAD CODE
==============================================================*/

/*
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
            ram->regs[i].type = ram->regs[j].type;
            vm_ram_free(ram, j, 0);
            i++;
        }
        j++;
    }
}


VMBlock * vm_ram_malloc_dynamic(VMRam* ram) {
    if(ram->used > ram->size - MEM_PAD) {
        vm_ram_grow(ram);
    }
    //ram->regs[ram->used].used = 1;
    return &ram->regs[ram->used++];
}

VMBlock * vm_ram_malloc_static(VMRam* ram, int index) {
    //printf("[STATIC] ALLOCATING.....\n");
    while(index > ram->size) vm_ram_grow(ram);
    ram->regs[index].ptr = 0;
    ram->regs[index].used = 1;

    ram->used = index;

    return &ram->regs[index];
}

void vm_ram_assign_dynamic(VMRam *ram, int value) {
    ram->used++;

    if(ram->used > ram->size - MEM_PAD) {
        ram->regs[ram->used].ptr = value;
        ram->regs[ram->used].used = 1;
        ram->regs[ram->used].type = VMInteger;

    }
    else {
        printf("[ASSIGN] WARNING - ALLOCATED TO BLOCK %-10i    [OKAY]\n", ram->used);
        VMBlock * a = vm_ram_malloc_dynamic(ram);
        a->ptr = value;
        a->used = 1;
        a->type = VMInteger;
    }
}
*/
#endif
