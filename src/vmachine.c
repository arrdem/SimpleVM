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

#include "vmemory.c"
#include "vio.c"
#include "vconsts.c"
#include "vmath.c"

#ifndef _VMACHINE_C_
#define _VMACHINE_C_

void vm_machine_print(VMachine* m) {
    int i = 0;
    while(i <= m->lines) {
        printf("%4i | %s\n",i, m->code[i++]);
    }
}

VMachine* vm_machine(FILE* source, int line_len) {
    int allocd_lines = 1;
    char c, *cursor, *alt_lines;

    VMachine* v = malloc(sizeof(VMachine));
    v->memory = vm_ram_init();
    v->cursor = 0;
    v->lines  = 0;

    v->code    = malloc(sizeof(char*) * allocd_lines);
    v->code[0] = malloc(sizeof(char) * line_len);
    cursor = v->code[0];

    while(c != EOF) {
        c = toupper(getc(source));
        if((c == '\n')
            || (c == '\r')) {
            *cursor = '\0';

            if(v->lines >= allocd_lines) {
                alt_lines = malloc(sizeof(char*) * 2 * allocd_lines);
                memcpy(alt_lines, v->code, allocd_lines);
                free(v->code);
                v->code = alt_lines;
            }

            v->lines++;
            v->code[v->lines] = malloc(sizeof(char) * line_len);
            cursor = v->code[v->lines];
        } else {
            *cursor = c;
            cursor++;
        }
    } *cursor = '\0';
    return v;
}

void vm_machine_run(VMachine* m) {
    srand(time(NULL));
    char *l;
    l = NULL;

    while(m->cursor < m->lines) {
        int i = 0, j = 0, k = 0;
        char *pch;

        if(l != NULL) free(l);
        l = malloc(sizeof(char) * 160);

        while(m->code[m->cursor][k] != '\0') {
            printf("<%i %c> ", k, m->code[m->cursor][k]);
            l[k] = m->code[m->cursor][k];
            k++;
        }

        //printf("[%10i] %s\n", m->cursor, m->code[m->cursor]);

        pch = strtok(l, " ");
        if(strcmp(l, "") == 0) continue;
        if(strcmp(pch, "") == 0) continue;
        else if(strcmp(pch, "LET") == 0) {
            // assign to address
            i = atoi(strtok(NULL, " "));
            j = atoi(strtok(NULL, " "));
            vm_ram_assign_static(m->memory, i, j);
            goto finally;

        } else if(strcmp(pch, "ADD") == 0) {
            i = atoi(strtok(NULL, " "));
            j = atoi(strtok(NULL, " "));
            k = atoi(strtok(NULL, " "));
            vm_math_add(m->memory, i, j, k);
            goto finally;

        } else if(strcmp(pch, "SUB") == 0) {
            // subtract
            i = atoi(strtok(NULL, " "));
            j = atoi(strtok(NULL, " "));
            k = atoi(strtok(NULL, " "));
            vm_math_sub(m->memory, i, j, k);
            goto finally;

        } else if(strcmp(pch, "DIV") == 0) {
            // divide
            i = atoi(strtok(NULL, " "));
            j = atoi(strtok(NULL, " "));
            k = atoi(strtok(NULL, " "));
            vm_math_div(m->memory, i, j, k);
            goto finally;

        } else if(strcmp(pch, "MUL") == 0) {
            // multiply
            i = atoi(strtok(NULL, " "));
            j = atoi(strtok(NULL, " "));
            k = atoi(strtok(NULL, " "));
            vm_math_mult(m->memory, i, j, k);
            goto finally;

        } else if(strcmp(pch, "MOD") == 0) {
            // modulus
            i = atoi(strtok(NULL, " "));
            j = atoi(strtok(NULL, " "));
            k = atoi(strtok(NULL, " "));
            vm_math_mod(m->memory, i, j, k);
            goto finally;

        } else if(strcmp(pch, "DEL") == 0) {
            // delete
            i = atoi(strtok(NULL, " "));
            vm_ram_free(m->memory, i, 1);
            goto finally;

        } else if(strcmp(pch, "DSP") == 0) {
            // print ram
            vm_ram_display(m->memory);
            goto finally;

        } else if(strcmp(pch, "HALT") == 0) {
            break;

        } else if(strcmp(pch, "RST") == 0) {
            // reset the VM's ram entirely
            vm_ram_rst(m->memory);

        } else if(strcmp(pch, "IF") == 0) {
            // the IF -> GOTO statement
            i = atoi(strtok(NULL, " "));
            j = atoi(strtok(NULL, " "));
            if(vm_ram_get(m->memory, i) > 0) {
                m->cursor = j;
                continue;  // do NOT run the cursor increment code
            } else goto finally;

        } else if(strcmp(pch, "GOTO") == 0) {
            // absolute jump
            i = atoi(strtok(NULL, " "));
            if(i <= m->lines) {
                m->cursor = i;
                continue;
            } else {
                printf("[FATAL] JUMPED OUT OF LINE BUFFER\n");
                exit(1);
            }

        } else if(strcmp(pch, "GO") == 0) {
            // relative jump
            i = atoi(strtok(NULL, " ")) + m->cursor;
            if((0 <= i) && (i <= m->lines)) {
                m->cursor = i;
                continue;
            } else {
                printf("[FATAL] JUMPED OUT OF LINE BUFFER\n");
                exit(1);
            }

        } else if(strcmp(pch, "RBIT") == 0) {
            // generate a random bit and save it.
            i = atoi(strtok(NULL, " "));
            vm_ram_assign_static(m->memory, i, (rand() % 2));
            goto finally;

        } else if(strcmp(pch, "RINT") == 0) {
            // generate a random bit and save it.
            i = atoi(strtok(NULL, " "));
            vm_ram_assign_static(m->memory, i, rand());
            goto finally;

        } else if(strcmp(pch, "PUT") == 0) {
            // print a single char
            i = atoi(strtok(NULL, " "));
            printf("%c", (char) vm_ram_get(m->memory, i));
            goto finally;

        } else if(strcmp(pch, "PUTS") == 0) {
            // print a string
            i = atoi(strtok(NULL, " "));

            char c;
            do {
                c = (char) vm_ram_get(m->memory, i++);
                printf("%c",c);
            } while (c != '\0');

            goto finally;

        } else if(strcmp(pch, "PUTI") == 0) {
            // print an integer
            printf("%i\n",vm_ram_get(m->memory, atoi(strtok(NULL, " "))));
            goto finally;

        } else {
            printf(
                "HERP DERP A DERP DE DERP\nUNRECOGNIZED: \"%s\"\n",
                pch);
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
