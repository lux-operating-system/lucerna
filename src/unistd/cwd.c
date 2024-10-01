/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

/* Implementation of unistd.h: Current working directory */

#include <unistd.h>
#include <limits.h>

char *getwd(char *path) {
    return getcwd(path, PATH_MAX);
}
