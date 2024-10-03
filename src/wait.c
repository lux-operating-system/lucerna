/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <sys/wait.h>

pid_t wait(int *status) {
    return waitpid(-1, status, 0);
}
