/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

/* Implementation of unistd.h: Environmental variables */

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char **environ;