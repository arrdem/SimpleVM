/*      main.c
 *
 *      Copyright 2011 Reid McKenzie <rmckenzie92@gmail.com>
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License as
 *      published by the Free Software Foundation; either version 2 of
 *      the License, or (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public
 *      License along with this program; if not, write to the Free
 *      Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *      Boston, MA 02110-1301, USA.
 *
 *      The main execution file of SimpleVM
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

    while(1) {
        int i = 0, j = 0, k = 0;
        char *s, *d, *pch;
        s = vm_io_read(stdin, ">>> \0", 80);

        if(!*s) return 0;

        d = s;
        while(*d != '\0') {
            *d = toupper((unsigned char)*d);
            d++;
        }

        pch = strtok(s, " \n\t");

        if(strcmp(pch, "LET") == 0) {
            // assign to address
            i = atoi(strtok(NULL, " "));
            j = atoi(strtok(NULL, " "));
            vm_ram_assign_static(ram, i, j);

        } else if(strcmp(pch, "ADD") == 0) {
            i = atoi(strtok(NULL, " "));
            j = atoi(strtok(NULL, " "));
            k = atoi(strtok(NULL, " "));
            vm_math_add(ram, i, j, k);

        } else if(strcmp(pch, "SUB") == 0) {
            // subtract
            i = atoi(strtok(NULL, " "));
            j = atoi(strtok(NULL, " "));
            k = atoi(strtok(NULL, " "));
            vm_math_sub(ram, i, j, k);

        } else if(strcmp(pch, "DIV") == 0) {
            // divide
            i = atoi(strtok(NULL, " "));
            j = atoi(strtok(NULL, " "));
            k = atoi(strtok(NULL, " "));
            vm_math_div(ram, i, j, k);

        } else if(strcmp(pch, "MUL") == 0) {
            // multiply
            i = atoi(strtok(NULL, " "));
            j = atoi(strtok(NULL, " "));
            k = atoi(strtok(NULL, " "));
            vm_math_mult(ram, i, j, k);

        } else if(strcmp(pch, "MOD") == 0) {
            // modulus
            i = atoi(strtok(NULL, " "));
            j = atoi(strtok(NULL, " "));
            k = atoi(strtok(NULL, " "));
            vm_math_mod(ram, i, j, k);

        } else if(strcmp(pch, "DEL") == 0) {
            // delete
            vm_ram_free(ram, atoi(strtok(NULL, " ")), 1);
        } else if(strcmp(pch, "DSP") == 0) {
            // print ram
            vm_ram_display(ram);
        } else if(strcmp(pch, "HLT") == 0) {
            break;
        } else if(strcmp(pch, "RST") == 0) {
            // reset the VM's ram entirely
            vm_ram_rst(ram);
        } else {
            printf("UNRECOGNIZED: \"%s\"\n",pch);
        }
    }
    printf("\nBye!\n");
    return 0;
}
