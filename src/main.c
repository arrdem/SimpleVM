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
#include "vmtypes.c"
#include "vmemory.c"
#include "vio.c"


int main(int argc, char **argv) {
    VMRam *ram;
    int j = 0;

    ram = vm_ram_init();

    vm_ram_rst(ram);

    while(j < 10) {
        vm_ram_assign(ram, j, j);
        j++;
    }

    vm_ram_free(ram, 3, 1);
    vm_ram_display(ram);
    printf("\nCOMPACTING...\n");
    vm_ram_compact(ram);
    vm_ram_display(ram);
    printf("\nGROWING....\n");
    vm_ram_grow(ram);
    vm_ram_display(ram);
    printf("\nDO SOME MATH...\n");
    j = 0;
    while(j < 10) {
        vm_math_mult(ram, j, j+1, j+10);
        j++;
    }
    vm_ram_display(ram);
    printf("\nLIKE A BAUS.\n");

    while(1) {
        char* s = vm_io_read(">>> \0", 80);
        char * pch;
        if(!*s) {
            return 0;
        }
        pch = strtok(s, " \n\t");

        if(strcmp(pch, "LET") == 0) {

            printf("ASSIGNING....\n");
        } else if(strcmp(pch, "ADD") == 0) {
            // add
            printf("ADDING....\n");
        } else if(strcmp(pch, "SUB") == 0) {
            // subtract
            printf("SUBTRACTING....\n");
        } else if(strcmp(pch, "DIV") == 0) {
            // divide
            printf("DIVIDING....\n");
        } else if(strcmp(pch, "MUL") == 0) {
            // multiply
        } else if(strcmp(pch, "MOD") == 0) {
            // modulus
        } else if(strcmp(pch, "DEL") == 0) {
            // delete
        } else if(strcmp(pch, "HLT") == 0) {
            break;
        } else {
            printf("UNRECOGNIZED: \"%s\"\n",pch);
        }
    }
    return 0;
}
