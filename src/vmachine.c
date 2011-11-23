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
                v->code[lines][j++] = '\0';
            }
        }
    }

    return v;
}

void vm_machine_run(VMachine* m) {
    while(1) {
        int i = 0, j = 0, k = 0;
        char *s, *d, *pch;
        s = m->code[m->cursor];

        if(!*s) goto finally;

        d = s;
        while(*d != '\0') {
            *d = toupper((unsigned char)*d);
            d++;
        }

        pch = strtok(l, " \n\t");

        if(strcmp(pch, "LET") == 0) {
            // assign to address
            i = atoi(strtok(NULL, " "));
            j = atoi(strtok(NULL, " "));
            vm_ram_assign(ram, i, j);

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

        finally: {
            m->cursor++;
            continue;
        }

        exception: {
            // deal with an error code here...
            printf("[run] ERROR ENCOUNTERED:\n\t%s", errmsg);

            switch(errcode) {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                default:
            }
        }
    }
}
