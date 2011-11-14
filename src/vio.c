#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **strings;
    int count;
} split;

char *vm_getline(char *line, int size)
{
    if (fgets(line, size, stdin)) {
        char *newline = strchr(line, '\n');
        if (newline) *newline =  '\0';
    }
    return line;
}

split* vm_split(char* line, const char delim) {
    split* s;
    s = malloc(sizeof(split));
    char* c = line;
    int i, j, str_len;
    s->count = 1;
    str_len = 0;

    while(*c != '\0') {
        printf("%c   ", *c);
        if(*c == delim) s->count++;
        str_len++;
        c++;
    }

    s->strings = malloc(s->count * sizeof(char*));

    printf("\nMALLOC OK: %i\n", s->count);
    while(i < s->count)
        s->strings[i++] = malloc((str_len - s->count) * sizeof(char));

    printf("RECURSIVE MALLOC OK: %i\n", (str_len - s->count));
    c = line;
    i = 0;
    j = 0;
    while(*c != '\0') {
        printf("I:%i J:%i C:%c, %X\n", i, j, *c, *c);
        if(*c == delim) {
            s->strings[i++][j] = '\0';
            j = 0;
        } else {
            s->strings[i][j++] = *c;
        }
        c++;
    }
    return s;
}

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
