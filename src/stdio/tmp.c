/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

static char __tmpnam[L_tmpnam];

char *tmpnam(char *s) {
    if(!s) s = __tmpnam;
    memset(s, 0, L_tmpnam);
    strcpy(s, "tmp-");

    for(int i = 4; i < L_tmpnam-1; i++) {
        int digit = rand() % 36;
        if(digit < 10) {
            s[i] = digit + '0';
        } else {
            if(rand() & 1) s[i] = digit + 'A' - 10;
            else s[i] = digit + 'a' - 10;
        }
    }

    if(!access(s, F_OK)) return tmpnam(s);
    return s;
}
