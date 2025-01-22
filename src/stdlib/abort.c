/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <signal.h>

void abort(void) {
    raise(SIGABRT);
}
