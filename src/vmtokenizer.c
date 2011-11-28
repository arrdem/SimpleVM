//      vmtokenizer.c
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//#include "getch.c"

char* CURR_TOK;
int   USED, INTEGER;

int getNextToken(FILE* is) {
        int quoted = 0, c, special, tok_len = 0;
        char d, *tok;
        fpos_t pos;

        tok = malloc(sizeof(char)*80);

        while(1) {
            fgetpos (is, &pos);
            c = getch();

            /*if (c == '"') quoted = !quoted;

            if(quoted) {
                if (c == '"') continue;
                else t.append((char) c);
            }*/

            // determine if we have a special character on our hands
            if((c == ' ') || (c == ';') || (c == '\0') || (c == '\n')) {
                   special = 1;
            } else special = 0;

            if(special && tok_len) {
                fsetpos(is, &pos);
                return new Token(t.toString());
            }
            else if((escape != 1) && tok.special()) {
                return tok;
            }
            else if((c == ' ') && (t.length() > 0)) {
                return new Token(t.toString());
            }
            else if(c == ' ') {
                continue;
            }
            else if(c == '\\') {

            }
            else {
                t.append((char) c);
            }
        }
        if(t.length() > 0) return new Token(t.toString());
        return null;
    }

char* vm_token_current() {
    return CURR_TOK;
}
