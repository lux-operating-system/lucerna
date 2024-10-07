/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

/* Implementation of unistd.h: Program execution */

#include <unistd.h>
#include <stdarg.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
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
    // for absolute paths just use execv()
    if(file[0] == '/' || file[0] == '.') return execv(file, argv);

    // for everything else, search the PATH
    char *pathenv = getenv("PATH");
    if(!pathenv) {
        errno = ENOENT;
        return -1;
    }

    char *program = malloc(PATH_MAX);
    if(!program) {
        errno = ENOMEM;
        return -1;
    }

    // follow the PATH in order
    char *token = strtok(pathenv, ":");
    while(token) {
        strcpy(program, token);
        strcpy(program+strlen(program), "/");
        strcpy(program+strlen(program), file);

        execv(program, argv);

        // if we're here, then that didn't work, move on to the next PATH entry
        // only if the error was that the file doesn't exist
        if(errno != ENOENT) return -1;
        token = strtok(NULL, ":");
    }

    // at this point the file probably doesn't exist
    // errno is already set by execv() so we can just return
    free(program);
    return -1;
}