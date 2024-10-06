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
#include <sys/stat.h>

static FILE _stdin = { .fd = STDIN_FILENO };
static FILE _stdout = { .fd = STDOUT_FILENO };
static FILE _stderr = { .fd = STDERR_FILENO };

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
        numMode = O_WRONLY | O_CREAT | O_TRUNC;
    else if(!strcmp(mode, "a") || !strcmp(mode, "ab"))
        numMode = O_WRONLY | O_CREAT | O_APPEND;
    else if(!strcmp(mode, "r+") || !strcmp(mode, "r+b") || !strcmp(mode, "rb+"))
        numMode = O_RDWR;
    else if(!strcmp(mode, "w+") || !strcmp(mode, "w+b") || !strcmp(mode, "wb+"))
        numMode = O_RDWR | O_CREAT | O_TRUNC;
    else if(!strcmp(mode, "a+") || !strcmp(mode, "a+b") || !strcmp(mode, "ab+"))
        numMode = O_RDWR | O_CREAT | O_APPEND;
    else if(!strcmp(mode, "wx") || !strcmp(mode, "wbx"))
        numMode = O_WRONLY | O_CREAT | O_TRUNC | O_EXCL;
    else if(!strcmp(mode, "w+x") || !strcmp(mode, "w+bx") || !strcmp(mode, "wb+x") || !strcmp(mode, "wbx+"))
        numMode = O_RDWR | O_CREAT | O_TRUNC | O_EXCL;

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

    file->eof = 0;
    file->error = 0;
    return file;
}

int fclose(FILE *file) {
    int status = close(file->fd);
    if(status) return EOF;
    else return 0;
}

int feof(FILE *file) {
    if(file->eof) return 1;

    struct stat st;
    if(fstat(file->fd, &st)) return 1;

    off_t position = lseek(file->fd, 0, SEEK_CUR);
    if(position < 0) return 1;

    file->eof = (position >= st.st_size);
    return file->eof;
}

size_t fwrite(const void *buffer, size_t size, size_t count, FILE *f) {
    size_t s = size*count;
    if(!s) return 0;

    f->error = 0;
    ssize_t status = write(f->fd, buffer, s);
    if(status > 0) return status / size;

    f->error = errno;
    return 0;
}

size_t fread(void *buffer, size_t size, size_t count, FILE *f) {
    size_t s = size*count;
    if(!s) return 0;

    f->error = 0;
    ssize_t status = read(f->fd, buffer, s);
    if(status > 0) return status / size;

    f->error = errno;
    return 0;
}

int puts(const char *s) {
    return fputs(s, stdout);
}

int fputs(const char *s, FILE *f) {
    int len = strlen(s) + 1;
    size_t status = fwrite(s, 1, strlen(s), f);
    if(!status || status == EOF) return EOF;
    fwrite("\n", 1, 1, f);

    return len;
}

int putc(int ch, FILE *f) {
    if(fwrite(&ch, 1, 1, f) == 1) return ch;
    else return EOF;
}

int putchar(int ch) {
    return putc(ch, stdout);
}

int fputc(int ch, FILE *f) {
    return putc(ch, f);
}

int getc(FILE *f) {
    int c = 0;
    if(fread(&c, 1, 1, f) == 1) return c;
    else return EOF;
}

int fgetc(FILE *f) {
    return getc(f);
}

ssize_t getdelim(char **lineptr, size_t *n, int delim, FILE *f) {
    if(!lineptr || !n) {
        errno = EINVAL;
        return -1;
    }

    if(!*lineptr) {
         if(*n) {
            *lineptr = malloc(*n);
         } else {
            *lineptr = malloc(4096);
            *n = 4096;
         }
    }

    if(!*lineptr) {
        errno = ENOMEM;
        return -1;
    }

    char *str = *lineptr;

    size_t counter = 0;
    for(;;) {
        int c = fgetc(f);
        if(c == delim) break;

        if(c == -1) continue;

        str[counter] = c;
        counter++;
        if(counter >= *n) {
            char *newptr = realloc(*lineptr, counter*2);
            if(!newptr) {
                errno = ENOMEM;
                return -1;
            }

            *lineptr = newptr;
            str = *lineptr;
        }
    }

    str[counter] = delim;
    str[counter+1] = 0;

    *n = counter+1;
    return *n;
}

ssize_t getline(char **lineptr, size_t *n, FILE *f) {
    return getdelim(lineptr, n, '\n', f);
}

int fseek(FILE *f, long offset, int where) {
    off_t s = lseek(f->fd, (off_t) offset, where);
    if(s < 0) return -1;
    return 0;
}

long ftell(FILE *f) {
    return (long) lseek(f->fd, 0, SEEK_CUR);
}

int fileno(FILE *f) {
    return f->fd;
}

int ferror(FILE *f) {
    return f->error;
}