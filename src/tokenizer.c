//      tokenizer.c
//
//      Copyright 2011 reid <reid@RMCKENZIE-UBUNTU-LAPTOP>
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.
//
//


#include <stdio.h>
#include <string.h>

#include "getch.c"

int main(int argc, char **argv) {
    int used = 0;
    char *l = malloc(sizeof(char) * 80),
         *c;

    while(1) {
        c = getch();
        if(c == NULL) {
            break;
        } if(c == ' ') {
            printf("TOKEN: %s\n", l);
            free(l);
            used = 0;
            l = malloc(sizeof(char) * 80);
        } else {
            l[used++] = c;
        }
    }

    return 0;
}

