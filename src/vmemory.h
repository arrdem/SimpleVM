/*      vmemory.h
 *      Copyright 2011 Reid McKenzie <rmckenzie92@gmail.com>
 *      This code and all other code in the project may be used
 *      or re-used for any purpose at all, so long as I am
 *      made aware of my contribution.
 *
 *      lays out the prototypes for the VM's memory
 */

#include <stdio.h>
#include <stdlib.h>

#include "vconsts.h"
#include "vmtypes.h"

VMRam * vm_ram_init();

void vm_ram_rst(VMRam*);

void vm_ram_display(VMRam*);
int vm_ram_get(VMRam*, int);

void vm_ram_grow(VMRam*);
void vm_ram_compact(VMRam*);
void vm_ram_free(VMRam*, int, int);

void vm_ram_assign_static(VMRam*, int, int);
void vm_ram_assign_dynamic(VMRam, int);

void vm_ram_addrmeta_type_set(VMRam*, int, int);
void vm_ram_addrmeta_used_set(VMRam*, int, int);
void vm_ram_addrmeta_heap_set(VMRam*, int, int);

int vm_ram_addrmeta_type(VMRam*, int);
int vm_ram_addrmeta_used(VMRam*, int);
int vm_ram_addrmeta_heap(VMRam*, int);

VMBlock * vm_ram_malloc_dynamic(VMRam*);
VMBlock * vm_ram_malloc_static(VMRam*, int);
