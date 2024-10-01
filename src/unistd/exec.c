/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

/* Implementation of unistd.h: Program execution */

#include <unistd.h>
#include <stdarg.h>
#include <errno.h>

/* execle(): execve() variant that loads args from va_args followed by an array
 * of environmental variables */

int execle(const char *path, const char *arg0, ...) {
    errno = ENOSYS;     // TODO
    return -1;
}

/* execl(): execle() but inheriting the environment of the parent */

int execl(const char *path, const char *arg0, ...) {
    errno = ENOSYS;     // TODO
    return -1;
}

/* execv(): execve() but inheriting the environment of the parent */

int execv(const char *path, char *const argv[]) {
    return execve(path, argv, environ);
}

/* execlp(): execl() but searches the PATH */

int execlp(const char *file, const char *arg0, ...) {
    errno = ENOSYS;     // TODO
    return -1;
}

/* execvp(): execv() but searches the PATH */

int execvp(const char *file, char *const argv[]) {
    errno = ENOSYS;     // TODO
    return -1;
}