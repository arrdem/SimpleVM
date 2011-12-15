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

#include "vmtypes.h"
#include "vmachine.h"

int main(int argc, char **argv) {
    VMachine* turing;
    turing = vm_machine(stdin);

    printf("TRYING TO PRINT MACHINE...\n");

    vm_machine_print(turing);

    printf("EVALUATING.....\n");

    //vm_machine_run(turing);

    return 0;
}
