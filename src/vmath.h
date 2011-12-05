#include <stdio.h>
#include <stdlib.h>

#include "vmtypes.h"
#include "vconsts.h"

void vm_math_generic(VMRam*, int, int, int, char);
void vm_math_add(VMRam *ram, int a, int b, int c);
void vm_math_sub(VMRam *ram, int a, int b, int c);
void vm_math_mult(VMRam *ram, int a, int b, int c);
void vm_math_div(VMRam *ram, int a, int b, int c);
void vm_math_mod(VMRam *ram, int a, int b, int c);

