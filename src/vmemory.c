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
        STAK MEMORY CODE
==============================================================*/
void vm_ram_display(VMRam* ram) {
    int i = 0;
    VMBlock b;
    while(i < ram->stack_size) {
        b = ram->stack[i];
        printf("[%-3i] used:%-5i type:%-8X data:%-8X %-10i\n",
               i++,
               vm_ram_stack_addrmeta_used(ram, i),
               vm_ram_stack_addrmeta_type(ram, i),
               b.ptr, b.ptr);
    }
}

int vm_ram_stack_get(VMRam* ram, int addr) {
    if((ram->stack_size >= addr) && vm_ram_stack_addrmeta_used(ram, addr)) {
        return ram->stack[addr].ptr;
    } else return 0;
}

void vm_ram_stack_free(VMRam* ram, int i, int j) {
    vm_ram_stack_addrmeta_used_set(ram, i, 0);
    vm_ram_stack_addrmeta_type_set(ram, i, 0);
    ram->stack[i].ptr =  VMNull;
}

void vm_ram_stack_grow(VMRam *ram) {
    //printf("[GROW] GROWING RAM....\n");
    int i = realloc(ram->stack, ram->stack_size * sizeof(VMBlock) * 2);
    if(i != NULL) {
        //printf("[GROW] SUCCESS.\n");
        ram->stack_size *= 2;
    } else {
        printf("[GROW] FAILURE IS FAIL. TERMINATING.\n");
        exit(1);
    }
}

// TODO - add the heap to this reset routine
void vm_ram_rst(VMRam *ram) {
    ram->stack_used = 0;
    //printf("[RST] USED                                          [OKAY]\n");

    if(ram->stack) {
        free(ram->stack);
    }
    //printf("[RST] FREE                                          [OKAY]\n");

    ram->stack = malloc(ram->stack_size * sizeof(VMBlock));
    //printf("[RST] MALLOC(%-10i)                            [OKAY]\n",(ram->stack_size));

    int i = ram->stack_size;
    while(i >= 0) {
        ram->stack[i].ptr  = &VMNull;
        vm_ram_stack_addrmeta_type_set(ram, i, 0);
        vm_ram_stack_addrmeta_used_set(ram, i, 0);
        i--;
    }
}

void vm_ram_stack_assign(VMRam *ram, int index, int value) {
    //printf("[STATIC] ASSIGNING.... \n");
    while(ram->stack_size < index) vm_ram_stack_grow(ram);
    if(!vm_ram_stack_addrmeta_used(ram, index)) vm_ram_stack_addrmeta_used_set(ram, index, 1);
    ram->stack[index].ptr = value;
    vm_ram_stack_addrmeta_type_set(ram, index, 1);
}

VMRam * vm_ram_init() {
    VMRam * ram;
    ram = malloc(sizeof(VMRam));

    ram->stack = malloc(sizeof(VMBlock) * MIN_MEM);
    memset(ram->stack, 0, sizeof(VMBlock) * MIN_MEM);
    ram->stack_size = MIN_MEM;

    ram->heap = malloc(MIN_MEM * sizeof(int*));
    ram->heap_size = MIN_MEM;
    ram->heap_used = 0;
    ram->heap_unused = stack_init();

    return ram;
}

void vm_ram_stack_addrmeta_type_set(VMRam *ram, int addr, int val) {
    ram->stack[addr].meta |= (val << 3);
}

void vm_ram_stack_addrmeta_used_set(VMRam *ram, int addr, int val) {
    ram->stack[addr].meta |= (val << 0);
}

void vm_ram_stack_addrmeta_heap_set(VMRam *ram, int addr, int val) {
    ram->stack[addr].meta |= (val << 4);
}

int vm_ram_stack_addrmeta_type(VMRam *ram, int addr) {
    return (ram->stack[addr].meta & (3 << 2));
}

int vm_ram_stack_addrmeta_used(VMRam *ram, int addr) {
    return (ram->stack[addr].meta & (1 << 0));
}

int vm_ram_stack_addrmeta_heap(VMRam *ram, int addr) {
    return (ram->stack[addr].meta & (1 << 4));
}

/*==============================================================
    HEAP MEMORY INTERACTION CODE
==============================================================*/



#endif
