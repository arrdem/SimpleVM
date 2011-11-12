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

const int VMTrue      = 1,
          VMFalse     = 0,
          VMNull      = 0xDEADBEEF,
          VMNotFound  = 0xFEEDFACE,
          MIN_MEM     = 15,
          VMPtr       = 0,
          VMInteger   = 0xAAAAAAAA,
          VMBoolean   = 0xBBBBBBBB,
          VMArray     = 0xCCCCCCCC,
          VMStack     = 0xDDDDDDDD;

typedef struct {
    int *ptr;
    int type;
    int addr;
    int used;
} VMBlock;

#define VMBlockSize sizeof(VMBlock);

VMBlock * REGS;
int FIRST_UNUSED_CELL = 0;
int MEM_SIZE;

void vm_display_ram() {
    int i = 0;
    VMBlock b;
    while(i < MEM_SIZE) {
        b = REGS[i];
        if(b.ptr != NULL) {
            printf("[%i] <used:%i type:%X data:%X, %i>\n", i, b.used, b.type, *b.ptr, *b.ptr);
        } else {
            printf("[%i] <used:%i type:%X data:NULL, NULL>\n", i, b.used);
        }
        i++;
    }
}

void vm_grow_ram() {
    VMBlock * newREGS;
    MEM_SIZE *= 2;
    newREGS = (VMBlock*) malloc(MEM_SIZE * sizeof(VMBlock));
    int i = 0;

    while(i < FIRST_UNUSED_CELL) {
        newREGS[i] = REGS[i];
        i++;
    }
    free(REGS);
    REGS = newREGS;
}

void vm_compact_ram() {
    int i = 0, j = 0;

    // find first un-used cell
    while(i < MEM_SIZE) {
        if(REGS[i].used == 0) {
            break;
        } i++;
    }

    // make both i and j the first junk index
    j = i;

    // find the next used cell, make it j
    while((REGS[j].used != 1) && (i < MEM_SIZE)) {
        j++;
    }

    // now copy the data left...
    while(j < MEM_SIZE) {
        if(REGS[j].used) {
            REGS[i] = REGS[j];
            vm_free(j);
            i++;
        }
        j++;
    }
}

int find (void* ptr) {
    int i = 0, found = 0;
    while(i < MEM_SIZE) {
        if(ptr == &REGS[i]) {
            found = 1;
            break;
        } else {
            i++;
        }
    }
    if(found) {
        return i;
    } else {
        return -1;
    }
}

VMBlock* vm_malloc(const int* a, int length) {
    if(FIRST_UNUSED_CELL + length >= MEM_SIZE) {
        vm_grow_ram();
    }
    return &REGS[FIRST_UNUSED_CELL += length];
}

void vm_free(int i) {
    if(REGS[i].ptr != NULL) {
        free(REGS[i].ptr);
    }
    REGS[i].used = VMFalse;
    REGS[i].type = VMNull;
    REGS[i].ptr = &VMNull;
}

void rst() {
    FIRST_UNUSED_CELL = 0;
    if(REGS != NULL) {free(REGS);}
    REGS = (VMBlock*) malloc(MEM_SIZE * sizeof(VMBlock));
    int i = 0;
    while(i < MEM_SIZE) {
        REGS[i].addr = i;
        REGS[i].ptr  = &VMNull;
        REGS[i].type = VMNull;
        REGS[i].used = VMFalse;
        i++;
    }
}

int main(int argc, char **argv) {
    VMBlock* k;
    int j = 0, *m;
    MEM_SIZE = MIN_MEM;

    rst();

    while(j < 10) {
        k = vm_malloc(VMInteger, 1);
        m = malloc(sizeof(int));
        k->ptr = m;
        *(m) = j;
        k->type = VMInteger;
        k->addr = j;
        k->used = 1;
        j++;
    }

    vm_free(3);
    vm_display_ram();
    vm_compact_ram();
    vm_display_ram();
    return 0;
}
