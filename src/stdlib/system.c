/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int system(const char *c) {
    if(!c) return 1;

    char *command = strdup(c);
    if(!command) return -1;

    int argc = 0;
    char **argv = NULL;
    char *token = strtok(command, " \n");
    while(token) {
        argv = realloc(argv, (argc+1) * sizeof(char *));
        if(!argv) {
            free(command);
            return -1;
        }

        argv[argc] = malloc(strlen(token) + 1);
        if(!argv[argc]) {
            free(command);
            return -1;
        }

        strcpy(argv[argc], token);

        token = strtok(NULL, " \n");
        argc++;
    }

    if(!argc) {
        free(argv);
        free(command);
        return 0;
    }

    argv = realloc(argv, (argc+1) * sizeof(char *));
    if(!argv) return -1;

    argv[argc] = NULL;  // null terminate

    pid_t child = fork();
    if(child < 0) {
        free(argv);
        free(command);
        return -1;
    }

    if(!child) {
        /* child */
        execvp(argv[0], argv);
        exit(-1);
    } else {
        /* parent */
        free(command);
        free(argv);
        int status;
        if(waitpid(child, &status, 0) != child) return -1;
        return status;
    }
}