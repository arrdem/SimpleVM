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

int vm_machine_hash(char* str) {
    // implimentation of the Java string hashing function
    char* s = &str;
    int h = 0;
    while(*s != '\0') {
        h = 31 * h + (int) *s;
        s++;
    }
    return h;
}

char* vm_machine_upper(char *str) {
    // (destructive?) string uppercase conversion function
    char* p = &str;
    while (*p = toupper(*p)) p++;
    return str;
}

void vm_machine_print(VMachine* m) {
    int k = 0, i;
    while(k <= m->lines) {
        printf("[%5i] %s ", k, &m->code[k].text, m->code[k].code[0]);
        i = 0;
        while(i < 7) {
            printf("%10i ", m->code[k].code[i]);
            i++;
        }
        printf("\n");
        k++;
    }
}

VMachine* vm_machine(FILE* stream) {
    int data_size = 2, data_used = 0, i = 0, f, k;
    VMachine* m;
    VMLine *data;

    m = malloc(sizeof(VMachine));
    m->memory = malloc(sizeof(VMBlock));
    m->lines = 0;

    data = malloc(sizeof(VMLine) * data_size);

    while(1) {
        if(data_size == data_used) {
            // grow VMLinebuffer
            VMLine* tmpVMLines = malloc(sizeof(VMLine) * 2 * data_size);
            memset(tmpVMLines, 0, sizeof(VMLine) * 2 * data_size);
            int q = 0;
            while(q < data_used) {
                tmpVMLines[q] = data[q++];
            }
            free(data);
            data = tmpVMLines;
            data_size *= 2;
        }

        data[data_used].code = malloc(7 * sizeof(int));

        data[data_used].text = malloc(sizeof(char)*10);
        f = fscanf(stream, "%s", &data[data_used].text);
        if(f == EOF) {
            memset(data[data_used].text, 0, sizeof(char) * 10);
            goto die;
        } else {
            data[data_used].text = vm_machine_upper(data[data_used].text);
             data[data_used].code[0] = vm_machine_hash(data[data_used].text);
        }

        k = 1;
        while(k < 7) {
            f = fscanf(stream, "%i", &data[data_used].code[k++]);
            if(f == EOF) {
                data[data_used].code[k-1] = 0;
                goto die;
            } else if(f == 0) {
                data[data_used].code[k-1] = 0;
            }
        }

        data_used++;
        continue;

        die:
            break;
    }

    m->lines = data_used;
    m->code = data;

    return m;
}

void vm_machine_eval(VMachine* m) {
    switch(m->code[m->cursor].code[0]) {
        default:
            break;
    }
}

#endif
