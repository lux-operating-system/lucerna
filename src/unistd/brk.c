/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

/* Implementation of unistd.h: Program break memory management */

#include <unistd.h>

int brk(void *addr) {
    void *current = sbrk(0);
    if(current == (void*)-1) return -1;     // errno is already set

    intptr_t delta = (intptr_t) addr - (intptr_t) current;
    if(sbrk(delta) == (void*)-1) return -1;
    else return 0;
}