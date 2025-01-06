/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

// Source:
// https://github.com/lattera/glibc/blob/master/stdlib/random_r.c#L364

#include <stdlib.h>

static unsigned __srand = 1;

void srand(unsigned seed) {
    __srand = seed;
}

int rand(void) {
    int val = ((__srand * 1103515245) + 12345);
    __srand = val;
    return val % RAND_MAX;
}
