#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vmtypes.c"

char *vm_io_getline(char *line, int size)
{
    if (fgets(line, size, stdin)) {
        char *newline = strchr(line, '\n');
        if (newline) *newline =  '\0';
    }
    return line;
}

char* vm_io_read(char* prompt, int line_len) {
    char* text;
    text = malloc(line_len * sizeof(char));

    printf("%s", prompt);
    vm_io_getline(text, sizeof(text));
    return text;
}
