/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <stdio.h>
#include <stdlib.h>

void __assert(long expr, const char *exprstr, const char *file, int line, const char *func) {
    if(expr) return;

    fprintf(stderr, "aborting due to failed assertion:\n");
    fprintf(stderr, " (%s) at %s:%d, in function %s\n", exprstr, file, line, func);

    abort();
}