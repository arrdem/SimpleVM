/*      vmemory.h
 *      Copyright 2011 Reid McKenzie <rmckenzie92@gmail.com>
 *      This code and all other code in the project may be used
 *      or re-used for any purpose at all, so long as I am
 *      made aware of my contribution.
 *
 *      lays out the prototypes for the VM's memory
 */

#ifndef vmemory_h
#define vmemory_h

#include <stdio.h>
#include <stdlib.h>

#include "vconsts.h"
#include "vmtypes.h"

VMRam *vm_ram_init                  ();
void vm_ram_rst                     (VMRam*);
void vm_ram_display                 (VMRam*);

int  vm_ram_stack_get               (VMRam*, int);
void vm_ram_stack_grow              (VMRam*);
void vm_ram_stack_free              (VMRam*, int, int);
void vm_ram_stack_assign            (VMRam*, int, int);
void vm_ram_stack_addrmeta_type_set (VMRam*, int, int);
void vm_ram_stack_addrmeta_used_set (VMRam*, int, int);
void vm_ram_stack_addrmeta_heap_set (VMRam*, int, int);
int  vm_ram_stack_addrmeta_type     (VMRam*, int);
int  vm_ram_stack_addrmeta_used     (VMRam*, int);
int  vm_ram_stack_addrmeta_heap     (VMRam*, int);

int  vm_ram_heap_get                (VMRam*, int);
void vm_ram_heap_grow               (VMRam*);
void vm_ram_heap_free               (VMRam*, int);
int  vm_ram_heap_malloc             (VMRam*, int);
void vm_ram_heap_free               (VMRam*, int);
#endif
