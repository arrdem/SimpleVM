#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vmtypes.c"
#include "vmutil.c"

char *vm_io_getline(char *line, int size)
{
    if (fgets(line, size, stdin)) {
        char *newline = strchr(line, '\n');
        if (newline) *newline =  '\0';
    }
    return line;
}

split* vm_io_read(char* prompt, int line_len) {
    char* text;
    text = malloc(line_len * sizeof(char));

    printf("%s", prompt);
    vm_io_getline(text, sizeof(text));
    return vm_util_split(text, ' ');
}

/*
int main(void)
{
    char text[80] = "";
    split* s;
    int i;

    fputs("prompt: ", stdout);
    fflush(stdout);
    printf("text = \"%s\"\n", vm_getline(text, sizeof text));
    s = vm_split(text, ' ');

    printf("ready to print..\n");
    while(i < s->count) {
        printf("%s\n", s->strings[i++]);
    }

    return 0;
}
*/
