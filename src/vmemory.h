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

VMBlock * vm_ram_malloc_dynamic(VMRam*);
VMBlock * vm_ram_malloc_static(VMRam*, int);
