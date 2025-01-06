/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <unistd.h>

unsigned sleep(unsigned seconds) {
    return usleep(seconds * 1000000);
}
