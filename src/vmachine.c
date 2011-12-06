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
#include <time.h>
#include <ctype.h>

#include "vmemory.h"
#include "vconsts.h"
#include "vmath.h"

#ifndef _VMACHINE_C_
#define _VMACHINE_C_

#include <ctype.h>

char* stoupper(char* s) {
    char* p = s;
    while (*p = toupper( *p )) p++;
    return s;
}

void vm_machine_print_instr(VMInstr *i, int* lineno) {
    printf("%4i | %s (%i %i %i %i %i %i)\n",
               *lineno,
               i->opcode,
               i->args[0],
               i->args[1],
               i->args[2],
               i->args[3],
               i->args[4],
               i->args[5]);
}

void vm_machine_print(VMachine* m) {
    int i = 0;
    while(i <= m->lines) {
        vm_machine_print_instr(&m->code[i], &i);
        i++;
    }
}

VMachine* vm_machine(FILE* source) {
    int allocd_lines = 8, cursor = 0, k = -1;
    VMachine* v = malloc(sizeof(VMachine));
    v->memory = vm_ram_init();
    v->cursor = 0;
    v->lines  = 0;
    v->code    = malloc(sizeof(VMInstr) * allocd_lines);

    while(1) {
        v->code[v->lines].opcode = malloc(sizeof(char) * 20);

        if (scanf("%s %i %i %i %i %i %i %i\n",
                 v->code[v->lines].opcode,
                 &v->code[v->lines].args[0],
                 &v->code[v->lines].args[1],
                 &v->code[v->lines].args[2],
                 &v->code[v->lines].args[3],
                 &v->code[v->lines].args[4],
                 &v->code[v->lines].args[5]) == EOF) break;

        v->code[v->lines].opcode = stoupper(v->code[v->lines].opcode);

        vm_machine_print_instr(&v->code[v->lines], &k);

        if(v->lines  == allocd_lines) {
            if(realloc(v->code, allocd_lines * 2 * sizeof(VMInstr))) {
                allocd_lines *= 2;
                printf("[INIT] GREW CODE ARRAY\n");
            } else {
                printf("[INIT] - FATAL - FAILED TO GROW CODE ARRAY\n");
                free(v->code);
                exit(1);
            }
        }
        v->lines++;
    }
    return v;
}

void vm_machine_run(VMachine* m) {
    srand(time(NULL));
    VMInstr i;

    while(m->cursor < m->lines) {
        i = m->code[m->cursor];
        printf("%4i | %s (%i %i %i %i %i %i)\n",
               m->cursor,
               i.opcode,
               i.args[0],
               i.args[1],
               i.args[2],
               i.args[3],
               i.args[4],
               i.args[5]);

        if(strcmp(i.opcode, "") == 0) continue;
        if(strcmp(i.opcode, "") == 0) continue;
        else if(strcmp(i.opcode, "LET") == 0) {
            // assign to address
            vm_ram_assign_static(m->memory, i.args[0], i.args[1]);
            goto finally;

        } else if(strcmp(i.opcode, "ADD") == 0) {
            // addition
            vm_math_add(m->memory, i.args[0], i.args[1], i.args[2]);
            goto finally;

        } else if(strcmp(i.opcode, "SUB") == 0) {
            // subtract
            vm_math_sub(m->memory, i.args[0], i.args[1], i.args[2]);
            goto finally;

        } else if(strcmp(i.opcode, "DIV") == 0) {
            // divide
            vm_math_div(m->memory, i.args[0], i.args[1], i.args[2]);
            goto finally;

        } else if(strcmp(i.opcode, "MUL") == 0) {
            // multiply
            vm_math_mult(m->memory, i.args[0], i.args[1], i.args[2]);
            goto finally;

        } else if(strcmp(i.opcode, "MOD") == 0) {
            // modulus
            vm_math_mod(m->memory, i.args[0], i.args[1], i.args[2]);
            goto finally;

        } else if(strcmp(i.opcode, "DEL") == 0) {
            // delete
            vm_ram_free(m->memory, i.args[0], 1);
            goto finally;

        } else if(strcmp(i.opcode, "DSP") == 0) {
            // print ram
            vm_ram_display(m->memory);
            goto finally;

        } else if(strcmp(i.opcode, "DUMP") == 0)  {
            // prints the code
            vm_machine_print(m);
            goto finally;

        } else if(strcmp(i.opcode, "HALT") == 0) {
            break;

        } else if(strcmp(i.opcode, "RST") == 0) {
            // reset the VM's ram entirely
            vm_ram_rst(m->memory);

        } else if(strcmp(i.opcode, "IF") == 0) {
            // the IF -> GOTO statement
            if(vm_ram_get(m->memory, i.args[0]) > 0) {
                m->cursor = i.args[1];
                continue;  // do NOT run the cursor increment code
            } else goto finally;

        } else if(strcmp(i.opcode, "GOTO") == 0) {
            // absolute jump
            if((i.args[0] <= m->lines) && (0 <= i.args[0])) {
                m->cursor = i.args[0];
                continue;
            } else {
                printf("[FATAL] JUMPED OUT OF LINE BUFFER\n");
                exit(1);
            }

        } else if(strcmp(i.opcode, "GO") == 0) {
            // relative jump
            if((0 <= i.args[0]) && (i.args[0] <= m->lines)) {
                m->cursor = i.args[0];
                continue;
            } else {
                printf("[FATAL] JUMPED OUT OF LINE BUFFER\n");
                exit(1);
            }

        } else if(strcmp(i.opcode, "RBIT") == 0) {
            // generate a random bit and save it.
            vm_ram_assign_static(m->memory, i.args[0], (rand() % 2));
            goto finally;

        } else if(strcmp(i.opcode, "RINT") == 0) {
            // generate a random bit and save it.
            vm_ram_assign_static(m->memory, i.args[0], rand());
            goto finally;

        } else if(strcmp(i.opcode, "PUT") == 0) {
            // print a single char
            printf("%c", (char) vm_ram_get(m->memory, i.args[0]));
            goto finally;

        } else if(strcmp(i.opcode, "PUTS") == 0) {
            // print a string
            int j =  i.args[0];
            char c;
            do {
                c = (char) vm_ram_get(m->memory,j++);
                printf("%c",c);
            } while (c != '\0');

            goto finally;

        } else if(strcmp(i.opcode, "PUTI") == 0) {
            // print an integer
            printf("%i\n",vm_ram_get(m->memory,  i.args[0]));
            goto finally;

        } else {
            printf(
                "HERP DERP A DERP DE DERP\nUNRECOGNIZED: \"%s\"\n",
                i.opcode);
            goto finally;
        }

        finally: {
            (m->cursor) += 1;
            continue;
        }

        die: {
            exit(1);
        }
    }
}

#endif
