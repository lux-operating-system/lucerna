/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <stdlib.h>

int abs(int n) {
    if(n < 0) return -1 * n;
    return n;
}
