/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <fcntl.h>

int creat(const char *path, mode_t mode) {
    return open(path, O_WRONLY | O_CREAT | O_TRUNC, mode);
}
