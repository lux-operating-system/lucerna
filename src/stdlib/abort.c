/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <stdlib.h>

void abort(void) {
    // TODO: reimplement a proper abort() after implementing signals
    exit(-1);
}
