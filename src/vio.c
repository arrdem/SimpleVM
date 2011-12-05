/*      vio.c
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
 *      Defines some simple IO functions used for reading code from
 *      STDIN and files. Will also contain the printing implimentation
 *      when I get around to writing it up.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vmtypes.h"

#ifndef _VIO_C_
#define _VIO_C_

char* vm_io_readf(FILE* file, int line_len) {
    char* line;
    line = malloc(sizeof(char) * line_len);
    fgets ( line, line_len, file );
    return line;
}

char* vm_io_read(FILE* file, char* prompt, int line_len) {
    printf("%s", prompt);
    return vm_io_readf(file, line_len);
}

#endif
