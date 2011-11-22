/*      vmachine.c
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
 *      Defines the loader code for the VMachine struct, and also the
 *      VMachine-based execution functions which actually perform
 *      VM instruction execution.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vmemory.c"
#include "vio.c"
#include "vconsts.c"

VMachine* vm_machine(FILE* source, int line_len) {
    int lines = 0, j = 0;
    int allocd_lines = 10;
    char *l, *pch;

    VMachine* v = malloc(sizeof(VMachine));
    v->memory = vm_ram_init();
    v->cursor = 0;
    v->code   = malloc(sizeof(char*) * allocd_lines);

    while((l = vm_io_readf(source, line_len)) != NULL) {
        lines++;

        if(lines >= allocd_lines) {
            // we need to grow the char array
            char* old_code = v->code;
            int i = allocd_lines, j = allocd_lines;
            i *= 2;
            v->code = malloc(sizeof(char*) * i);

            while(j >= 0) {
                // copy the pointers...
                v->code[j] = old_code[j];
                j--;
            }

            free(old_code); // take out the trash;
        }

        v->code[lines] = malloc(sizeof(char*) * LN_TOKS);

        while(j < LN_TOKS) {
            pch = strtok(l, " \n\t");
            if(pch != NULL) {
                v->code[lines][j++] = pch;
            } else {
                v->code[lines][j] = malloc(sizeof(char));
                *v->code[lines][j++] = '\0';
            }
        }
    }

    return v;
}
