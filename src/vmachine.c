/*      vmachine.c
 *      Copyright 2011 Reid McKenzie <rmckenzie92@gmail.com>
 *      This code and all other code in the project may be used
 *      or re-used for any purpose at all, so long as I am
 *      made aware of my contribution.
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
#include "linklist.h"

#ifndef _VMACHINE_C_
#define _VMACHINE_C_

long vm_machine_string_hash(char* str) {
    // the Java string hashing function
    long h = 0;
    while(*str != '\0') {
        h = 31 * h + (int) *str;
        str++;
    }
    return h;
}

char* vm_machine_upper(char *str) {
    // (destructive?) string uppercase conversion function
    char* f = str;
    while (*str = toupper(*str)) str++;
    return f;
}

void vm_machine_print(VMachine* m) {
    int k = 0, i;
    while(k <= m->lines) {
        printf("[%-3i] %5s ", k, m->code[k].text, m->code[k].code[0]);
        i = 0;
        while(i < 7) {
            printf("%-10i ", m->code[k].code[i]);
            i++;
        }
        printf("\n");
        k++;
    }
}

void vm_machine_delete(VMachine* m) {
    int i = 0;
    while(i < m->lines) {
        free(m->code[i].text);
        free(m->code[i++].code);
    }
    free(m->code);
    free(m);
}

VMThread* vm_thread(int line, int id) {
    VMThread *t = malloc(sizeof(VMThread));
    t->id = id;
    t->line = line;
    return t;
}

VMachine* vm_machine_binary(FILE* stream) {
    int data_size = 2, data_used = 0, i = 0;
    int header[VMHeaderSize];
    long fsize;
    VMachine* m;
    VMLine *data;

    m = malloc(sizeof(VMachine));

    m->memory = vm_ram_init();
    m->threads = malloc(sizeof(ll));

    m->threads->next = m->threads;
    m->threads->data = malloc(sizeof(VMThread));

    VMThread* t = m->threads->data;
    t->line = 0;
    t->id = 1;

    m->lines = 0;
    m->threadcount = 1;

    data = malloc(sizeof(VMLine) * data_size);

    fseek(stream, 0L, SEEK_END);
    fsize = ftell(stream);
    rewind(stream);

    fread(header, sizeof(int), VMHeaderSize, stream);

    if((header[0] != VMMajorVersion) ||
       (header[1] != VMMinorVersion))  {
        printf("[INIT] WARNING - BYTECODE WAS BUILT FOR A DIFFERENT VERSION\n");
        printf("          VM VERSION %i.%i\n", VMMajorVersion, VMMinorVersion);
        printf("      TARGET VERSION %i.%i\n", header[0], header[1]);
        printf("      ATTEMPTING TO CONTINUE... RESULTS NOT GUARANTEED\n");
    } else if(((fsize - VMHeaderSize*sizeof(int)) %
               (7*sizeof(int))) != 0) {
        printf("[INIT] WARNING - BYTECODE HAS A STRANGE SIZE\n");
        printf("      SIZE:%i\n", fsize);
        printf(" INT COUNT:%i\n", (fsize - VMHeaderSize*sizeof(int)/sizeof(int)));
    } else {
        printf("[INIT] INSTRUCTION LOAD...                                               [OKAY]\n");
    }

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

        data[data_used].code = malloc(sizeof(int)  * 7);
        data[data_used].text = malloc(sizeof(char) * 4);
        strcpy(data[data_used].text, "---\0");

        i = fread(data[data_used].code, sizeof(int), 7, stream);
        /*printf("%i %i %i %i %i %i %i\n",
               data[data_used].code[0],
               data[data_used].code[1],
               data[data_used].code[2],
               data[data_used].code[3],
               data[data_used].code[4],
               data[data_used].code[5],
               data[data_used].code[6]);*/

        if(i > 0) {
            data_used++;
            continue; // not there yet....
        } else {
            // clean up the mess
            memset(data[data_used].code, 0, 7);
            break; // and quit
        }
    }

    m->lines = data_used;
    m->code = data;

    return m;
}

VMachine* vm_machine_ascii(FILE* stream) {
    int data_size = 2, data_used = 0, i = 0, f, k;
    VMachine* m;
    VMLine *data;

    m = malloc(sizeof(VMachine));

    m->memory = vm_ram_init();
    m->threads = malloc(sizeof(ll));

    m->threads->next = m->threads;
    m->threads->data = malloc(sizeof(VMThread));

    VMThread* t = m->threads->data;
    t->line = 0;
    t->id = 1;

    m->lines = 0;
    m->threadcount = 1;

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
        f = fscanf(stream, "%s", data[data_used].text);
        if(f == EOF) {
            memset(data[data_used].text, 0, sizeof(char) * 10);
            goto die;
        }
        else {
            data[data_used].text = vm_machine_upper(data[data_used].text);
            data[data_used].code[0] = vm_machine_string_hash(data[data_used].text);
        }

        k = 1;
        while(k < 7) {
            f = fscanf(stream, "%i", &data[data_used].code[k++]);
            if(f == EOF) {
                data[data_used].code[k-1] = 0;
                goto die;
            }
            else if(f == 0) {
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

void vm_machine_thread_del(VMachine *m, int id) {
    ll *c, *t;
    VMThread *y;
    t = m->threads;
    c = t->next;
    y = (VMThread*) c->data;
    while((y->id != id)&&(c->next)&&(c != m->threads)) {
        t = c;
        c = c->next;
        y = (VMThread*) c->data;
    }
    if(y->id == id) {
        t->next = c->next;
        free(c);
    }
}

int vm_machine_eval(VMachine* m, int line) {
    if((line < 0) || (line > m->lines)) {
        // out of line buffer error state
        m->errcode = 1;
        m->errmsg = "CURSOR OUT OF LINE BUFFER";
    }
    else {
        srand(time(NULL));
        int i;
        switch(m->code[line].code[0]) {


            case 75259:
                // LET N1 N2
                vm_ram_stack_assign(m->memory,
                                     m->code[line].code[1],
                                     m->code[line].code[2]);
                break;

            case 64641:
                // ADD N1 N2 N3
                i = (vm_ram_stack_get(m->memory,
                                m->code[line].code[1]) +
                                vm_ram_stack_get(m->memory,
                                           m->code[line].code[2]));

                if ((vm_ram_stack_get(m->memory, m->code[line].code[1]) &&
                        vm_ram_stack_get(m->memory, m->code[line].code[2]) &&
                        (i < 0)) ||
                    (!vm_ram_stack_get(m->memory, m->code[line].code[1]) &&
                        !vm_ram_stack_get(m->memory, m->code[line].code[2]) &&
                        (i > 0))) {
                    // an integer overflow has occured...
                    m->errcode = 2;
                    m->errmsg = "INTEGER OVERFLOW";
                    goto finally;
                }
                else {
                    // the addition is fine...
                    vm_ram_stack_assign(m->memory,
                                         m->code[line].code[3], i
                                         );
                }
                break;

            case 82464:
                // SUB N1 N2 N3
                i = (vm_ram_stack_get(m->memory,
                                m->code[line].code[1]) -
                                vm_ram_stack_get(m->memory,
                                           m->code[line].code[2]));

                if ((vm_ram_stack_get(m->memory, m->code[line].code[1]) &&
                        vm_ram_stack_get(m->memory, m->code[line].code[2]) &&
                        (i > 0)) ||
                    (!vm_ram_stack_get(m->memory, m->code[line].code[1]) &&
                        !vm_ram_stack_get(m->memory, m->code[line].code[2]) &&
                        (i < 0))) {
                    // an integer overflow has occured...
                    m->errcode = 2;
                    m->errmsg = "INTEGER UNDERFLOW";
                    goto finally;
                }
                else {
                    // the addition is fine...
                    vm_ram_stack_assign(m->memory,
                                         m->code[line].code[3], i
                                         );
                }
                break;

            case 76708:
                // MUL N1 N2 N3
                i = (vm_ram_stack_get(m->memory,
                                m->code[line].code[1]) *
                                vm_ram_stack_get(m->memory,
                                           m->code[line].code[2]));

                if ((!vm_ram_stack_get(m->memory, m->code[line].code[1]) &&
                        vm_ram_stack_get(m->memory, m->code[line].code[2]) &&
                        (i > 0)) ||
                    (vm_ram_stack_get(m->memory, m->code[line].code[1]) &&
                        !vm_ram_stack_get(m->memory, m->code[line].code[2]) &&
                        (i > 0))) {
                    // an integer overflow has occured...
                    m->errcode = 2;
                    m->errmsg = "INTEGER UNDERFLOW";
                    goto finally;
                }
                else {
                    // the addition is fine...
                    vm_ram_stack_assign(m->memory,
                                         m->code[line].code[3], i
                                         );
                }
                break;

            case 67697:
                // DIV N1 N2 N3
                i = (vm_ram_stack_get(m->memory,
                                m->code[line].code[1]) /
                                vm_ram_stack_get(m->memory,
                                           m->code[line].code[2]));

                if ((!vm_ram_stack_get(m->memory, m->code[line].code[1]) &&
                        vm_ram_stack_get(m->memory, m->code[line].code[2]) &&
                        (i > 0)) ||
                    (vm_ram_stack_get(m->memory, m->code[line].code[1]) &&
                        !vm_ram_stack_get(m->memory, m->code[line].code[2]) &&
                        (i > 0))) {
                    // an integer overflow has occured...
                    m->errcode = 2;
                    m->errmsg = "INTEGER UNDERFLOW";
                    goto finally;
                }
                else {
                    // the addition is fine...
                    vm_ram_stack_assign(m->memory,
                                         m->code[line].code[3], i
                                         );
                }
                break;

            case 76514:
                // MOD N1 N2 N3
                vm_ram_stack_assign(m->memory,
                                     m->code[line].code[3],
                                     (vm_ram_stack_get(m->memory,
                                                   m->code[line].code[1]) %
                                        vm_ram_stack_get(m->memory,
                                                   m->code[line].code[2])
                                     ));
                break;

            case 2285:
                // GT N1 N2 N3
                vm_ram_stack_assign(m->memory,
                                     m->code[line].code[3],
                                     (vm_ram_stack_get(m->memory,
                                                   m->code[line].code[1]) >
                                        vm_ram_stack_get(m->memory,
                                                   m->code[line].code[2])
                                     ));
                break;

            case 2440:
                vm_ram_stack_assign(m->memory,
                                     m->code[line].code[3],
                                     (vm_ram_stack_get(m->memory,
                                                   m->code[line].code[1]) <
                                        vm_ram_stack_get(m->memory,
                                                   m->code[line].code[2])
                                     ));
                break;

            case 66219796:
                vm_ram_stack_assign(m->memory,
                                     m->code[line].code[3],
                                     (vm_ram_stack_get(m->memory,
                                                   m->code[line].code[1]) ==
                                        vm_ram_stack_get(m->memory,
                                                   m->code[line].code[2])
                                     ));
                break;

            case 68001:
                // DSP
                vm_ram_display(m->memory);
                break;

            case 2109940:
                // DUMP
                vm_machine_print(m);
                break;

            case 2467642:
                // PUTI N1
                printf("[PUTI LINE %i] %i\n", line, vm_ram_stack_get(m->memory, m->code[line].code[1]));
                break;

            case 2467636:
                // PUTC N1
                printf("%c\n", line, vm_ram_stack_get(m->memory, m->code[line].code[1]));
                break;

            case 2184147:
                // GETI N1
                printf("[GETI LINE %i - REG %i] > ", line,  m->code[line].code[1]);
                scanf("%i", i);
                vm_ram_stack_assign(m->memory,
                                     m->code[line].code[1],
                                     i);
                break;

            case 2184141:
                //GETC N1
                printf("[GETC LINE %i - REG %i] > ", line, m->code[line].code[1]);
                scanf("%c", i);
                vm_ram_stack_assign(m->memory,
                                     m->code[line].code[1],
                                     i);
                break;

            case 2531:
                // OR N1 N2 N3
                vm_ram_stack_assign(m->memory,
                                     m->code[line].code[3],
                                     (vm_ram_stack_get(m->memory,
                                                   m->code[line].code[1]) ||
                                        vm_ram_stack_get(m->memory,
                                                   m->code[line].code[2])
                                     ));
                break;

            case 64951:
                // AND N1 N2 N3
                vm_ram_stack_assign(m->memory,
                                     m->code[line].code[3],
                                     (vm_ram_stack_get(m->memory,
                                                   m->code[line].code[1]) &&
                                        vm_ram_stack_get(m->memory,
                                                   m->code[line].code[2])
                                     ));
                break;

            case 87099:
                // XOR N1 N2 N3le
                vm_ram_stack_assign(m->memory,
                                     m->code[line].code[3],
                                     (vm_ram_stack_get(m->memory,
                                                   m->code[line].code[1]) ^
                                        vm_ram_stack_get(m->memory,
                                                   m->code[line].code[2])
                                     ));
                break;

            case 77491:
                // NOT N1 N2
                vm_ram_stack_assign(m->memory,
                                     m->code[line].code[2],
                                     (!vm_ram_stack_get(m->memory,
                                                   m->code[line].code[1])));
                break;

            case 2193763:
                // GOTO N1 (go to line C+N1)
                line = line + m->code[line].code[1];
                return line;

            case 68006735:
                // GOTOR N1 (go to line number in register)
                line = vm_ram_stack_get(m->memory, m->code[line].code[1]);
                return line;

            case 68006729:
                // GOTOL N1 (go to absolute line number (indexed from 0))
                line = m->code[line].code[1];
                return line;

            case 2333:
                // IF N1
                if(vm_ram_stack_get(m->memory, m->code[line].code[1])) {
                    line += 1;
                }
                else {
                    line += 2;
                }
                return line;

            case 2558355:
                // SWAP N1 N2
                // XOR the pointers!
                i = vm_ram_stack_get(m->memory, m->code[line].code[1]);
                vm_ram_stack_assign(m->memory,
                                     m->code[line].code[1],
                                     vm_ram_stack_get(m->memory,
                                                m->code[line].code[2]));
                vm_ram_stack_assign(m->memory,
                                     m->code[line].code[1],
                                     i);

                break;

            case 2251860:
                // add 1 to the register
                vm_ram_stack_assign(m->memory,
                                     m->code[line].code[1],
                                     (vm_ram_stack_get(m->memory,
                                                 m->code[line].code[1])
                                        + 1)
                                     );
                break;

            case 2094256:
                // subtract 1
                vm_ram_stack_assign(m->memory,
                                     m->code[line].code[1],
                                     (vm_ram_stack_get(m->memory,
                                                 m->code[line].code[1])
                                        - 1)
                                     );
                break;

            case 2360863:
                // MCPY N1 N2
                // TODO - ADD METADATA COPY
                vm_ram_stack_assign(m->memory,
                                     m->code[line].code[2],
                                     (vm_ram_stack_get(m->memory,
                                                 m->code[line].code[1]))
                                     );
                break;

            case 2513316:
                // RGET N1 N2
                i = vm_ram_stack_get(m->memory, m->code[line].code[1]);
                if(i >= 0) {
                    // STACK CASE
                    vm_ram_stack_assign(m->memory,
                                         m->code[line].code[2],
                                         vm_ram_stack_get(m->memory,i)
                                         );
                } else {
                    // HEAP CASE

                }
                break;

            case 2524848:
                // RSET N1 N2
                vm_ram_stack_assign(m->memory,
                                     (vm_ram_stack_get(m->memory,
                                                 vm_ram_stack_get(m->memory,
                                                            m->code[line].code[2]))),
                                     m->code[line].code[1]
                                     );
                break;

            case 2163906:
                // FORK N1
                // creates a new "thread" (really a cursor) on line N1
                ll_insert(m->threads,
                          vm_thread(m->code[line].code[1],
                                    m->threadcount+1)
                          );
                vm_ram_stack_assign(m->memory,
                                     m->code[line].code[2],
                                     (m->threadcount++)+1);
                m->threadcount++;
                break;

            case 2282794:
                // JOIN N1
                // kills the thread with the ID at register N1
                i = vm_ram_stack_get(m->memory,
                               m->code[line].code[1]);

                m->threadcount--;
                if(i) vm_machine_thread_del(m,i);
                else return -1;
                break;

            case 2402146:
                // NOOP
                break;

            case 2508635:
                // RBIT N1 - get a random 1 or 0
                vm_ram_stack_assign(m->memory,
                                     m->code[line].code[1],
                                     rand()%2);
                break;

            case 2515517:
                // RINT N1 - get a random int, save to N1
                vm_ram_stack_assign(m->memory,
                                     m->code[line].code[1],
                                     rand());
                break;

            case 2094719:
                // HERP DERP
                printf("HERP DERP A DERP DE DEEEEEEEERP\n");
                exit(9001);
                break;

            case 2209857:
                // HALT
                exit(m->code[line].code[1]); // DIE DIE DIE!!!111!1!!!1
                break;

            default:
                printf("ERROR ON LINE %i - UNRECOGNIZED INSTRUCTION - %i\n", line, m->code[line].code[0]);
                break;
        }

        finally:
        return line+1;
    }
}

void vm_machine_run(VMachine* m) {
    int i;
    VMThread* t;
    ll* cursor;
    cursor = m->threads;
    while(m->threadcount) {
        t = (VMThread*) cursor->data;
        if(!t) exit(1);
        //printf("[EVAL THREAD %i] ", t->id);
        i = vm_machine_eval(m, t->line);
        if((cursor->data) && (i >= 0) && (m->errcode == 0)) {
            t->line = i;
            cursor = cursor->next;
        }
        else if(i == -1) {
            // this is the "kill me" code
            vm_machine_thread_del(m,
                                  t->id);
        }
        else {
            // deal with error code... how?
            printf("[ERROR ON LINE %i] %s", t->line, m->errmsg);
            exit(1); // die!
        }
    }
}

#endif
