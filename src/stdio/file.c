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

static FILE _stdin = { .fd = 0 };
static FILE _stdout = { .fd = 1 };
static FILE _stderr = { .fd = 2 };

FILE *stdin = &_stdin;
FILE *stdout = &_stdout;
FILE *stderr = &_stderr;

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

size_t fwrite(const void *buffer, size_t size, size_t count, FILE *f) {
    size_t s = size*count;
    if(!s) return 0;

    ssize_t status = write(f->fd, buffer, s);
    if(status > 0) return status;
    else return 0;
}

size_t fread(void *buffer, size_t size, size_t count, FILE *f) {
    size_t s = size*count;
    if(!s) return 0;

    ssize_t status = read(f->fd, buffer, s);
    if(status > 0) return status;
    else return 0;
}

int puts(const char *s) {
    int len = strlen(s) + 1;
    size_t status = fwrite(s, 1, strlen(s), stdout);
    if(!status || status == EOF) return EOF;
    fwrite("\n", 1, 1, stdout);

    return len;
}