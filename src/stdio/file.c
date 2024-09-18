/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fnctl.h>
#include <errno.h>
#include <unistd.h>

FILE *fopen(const char *path, const char *mode) {
    if(!strlen(path)) {
        errno = ENOENT;
        return NULL;
    }

    if(!strlen(mode)) {
        errno = EINVAL;
        return NULL;
    }

    int numMode = 0;
    mode_t defaultMode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;

    if(!strcmp(mode, "r") || !strcmp(mode, "rb"))
        numMode = O_RDONLY;
    else if(!strcmp(mode, "w") || !strcmp(mode, "wb"))
        numMode = O_WRONLY | O_CREAT;
    else if(!strcmp(mode, "a") || !strcmp(mode, "ab"))
        numMode = O_WRONLY | O_APPEND;
    else if(!strcmp(mode, "r+") || !strcmp(mode, "r+b") || !strcmp(mode, "rb+"))
        numMode = O_RDWR | O_APPEND;
    else if(!strcmp(mode, "w+") || !strcmp(mode, "w+b") || !strcmp(mode, "wb+"))
        numMode = O_RDWR | O_CREAT;
    else if(!strcmp(mode, "a+") || !strcmp(mode, "a+b") || !strcmp(mode, "ab+"))
        numMode = O_RDWR | O_CREAT | O_APPEND;
    else if(!strcmp(mode, "wx") || !strcmp(mode, "wbx"))
        numMode = O_WRONLY | O_CREAT | O_EXCL;
    else if(!strcmp(mode, "w+x") || !strcmp(mode, "w+bx") || !strcmp(mode, "wb+x") || !strcmp(mode, "wbx+"))
        numMode = O_RDWR | O_CREAT | O_EXCL;

    if(!numMode) {
        errno = EINVAL;
        return NULL;
    }

    FILE *file = malloc(sizeof(FILE));
    if(!file) {
        errno = ENOMEM;
        return NULL;
    }

    file->fd = open(path, numMode, defaultMode);
    if(file->fd < 0) {
        free(file);
        return NULL;
    }

    return file;
}

int fclose(FILE *file) {
    int status = close(file->fd);
    if(status) return EOF;
    else return 0;
}
