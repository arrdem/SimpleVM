/*      main.c
 *
 *      Copyright 2011 Reid McKenzie <rmckenzie92@gmail.com>
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 *
*/

#include <stdio.h>
#include <stdlib.h>

#include "vmath.c"
#include "vconsts.c"
#include "vmblock.c"
#include "vmemory.c"

int main(int argc, char **argv) {
    VMBlock* k;
    VMRam *ram;
    int j = 0, *m;

    ram = vm_ram_init();
    printf("%i\n", ram->size);

    vm_ram_rst(ram);

    while(j < 10) {
        printf("%i\n", j);
        k = vm_ram_malloc(ram);
        m = malloc(sizeof(int));
        k->ptr = m;
        *(m) = j;
        k->type = VMInteger;
        k->addr = j;
        k->used = 1;
        j++;
    }

    vm_ram_free(ram, 3, 1);
    vm_ram_display(ram);
    printf("\nCOMPACTING...\n");
    vm_ram_compact(ram);
    vm_ram_display(ram);
    printf("\nGROWING....\n");
    vm_ram_grow(&ram);
    vm_ram_display(ram);
    printf("\nLIKE A BAUS.\n");
    return 0;
}
