/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/mman.h>

static FILE _stdin = {
    .fd = STDIN_FILENO, .mmap = NULL, .error = 0,
    .eof = 0, .bufferType = _IOLBF, .bufferSize = 0
};

static FILE _stdout = {
    .fd = STDOUT_FILENO, .mmap = NULL, .error = 0,
    .eof = 0, .bufferType = _IOLBF, .bufferSize = 0
};

static FILE _stderr = {
    .fd = STDERR_FILENO, .mmap = NULL, .error = 0,
    .eof = 0, .bufferType = _IONBF, .bufferSize = 0
};

FILE *stdin = &_stdin;
FILE *stdout = &_stdout;
FILE *stderr = &_stderr;

static FILE **_openFiles = NULL;
static int _openFileCount = 3;  // stdio

FILE *fopen(const char *path, const char *mode) {
    if(_openFileCount >= OPEN_MAX) {
        errno = ENFILE;
        return NULL;
    }

    if(!path || !strlen(path)) {
        errno = ENOENT;
        return NULL;
    }

    if(!mode || !strlen(mode)) {
        errno = EINVAL;
        return NULL;
    }

    if(!_openFiles) {
        _openFiles = calloc(OPEN_MAX, sizeof(FILE *));
        if(!_openFiles) {
            errno = ENOMEM;
            return NULL;
        }

        _openFiles[0] = stdin;
        _openFiles[1] = stdout;
        _openFiles[2] = stderr;
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

    FILE *file = calloc(1, sizeof(FILE));
    if(!file) {
        errno = ENOMEM;
        return NULL;
    }

    file->fd = open(path, numMode, defaultMode);
    if(file->fd < 0) {
        free(file);
        return NULL;
    }

    file->bufferType = _IOFBF;

    for(int i = 0; i < OPEN_MAX; i++) {
        if(!_openFiles[i]) _openFiles[i] = file;
    }

    _openFileCount++;

#ifdef STDIO_USE_MMAP
    struct stat st;
    if(fstat(file->fd, &st) || (!S_ISREG(st.st_mode)))
        return file;

    int prot = 0;
    if((numMode & O_RDWR) == O_RDWR) prot = PROT_READ | PROT_WRITE;
    else if(numMode & O_RDONLY) prot = PROT_READ;
    else if(numMode & O_WRONLY) prot = PROT_WRITE;

    file->mmap = mmap(NULL, st.st_size, prot, MAP_SHARED, file->fd, 0);
    if(file->mmap == MAP_FAILED) {
        file->mmap = NULL;
        return file;
    }

    file->mmapLength = st.st_size;
    file->position = lseek(file->fd, 0, SEEK_CUR);
    close(file->fd);
    file->fd = -1;
#endif

    return file;
}

FILE *fdopen(int fd, const char *mode) {
    if(_openFileCount >= OPEN_MAX) {
        errno = ENFILE;
        return NULL;
    }

    if(!mode || !strlen(mode)) {
        errno = EINVAL;
        return NULL;
    }

    if(!_openFiles) {
        _openFiles = calloc(OPEN_MAX, sizeof(FILE *));
        if(!_openFiles) {
            errno = ENOMEM;
            return NULL;
        }

        _openFiles[0] = stdin;
        _openFiles[1] = stdout;
        _openFiles[2] = stderr;
    }

    int numMode = 0;
    if(!strcmp(mode, "r") || !strcmp(mode, "rb"))
        numMode = O_RDONLY;
    else if(!strcmp(mode, "w") || !strcmp(mode, "wb"))
        numMode = O_WRONLY | O_CREAT;
    else if(!strcmp(mode, "a") || !strcmp(mode, "ab"))
        numMode = O_WRONLY | O_CREAT | O_APPEND;
    else if(!strcmp(mode, "r+") || !strcmp(mode, "r+b") || !strcmp(mode, "rb+"))
        numMode = O_RDWR;
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

    int flags = fcntl(fd, F_GETFL);
    if(flags < 0) return NULL;

    if((flags & O_RDONLY) != (numMode & O_RDONLY)) {
        errno = EINVAL;
        return NULL;
    }

    if((flags & O_WRONLY) != (numMode & O_WRONLY)) {
        errno = EINVAL;
        return NULL;
    }

    if((flags & O_RDWR) != (numMode & O_RDWR)) {
        errno = EINVAL;
        return NULL;
    }

    int newFlags = flags;
    if(numMode & O_APPEND) newFlags |= O_APPEND;
    else newFlags &= ~O_APPEND;

    FILE *file = calloc(1, sizeof(FILE));
    if(!file) {
        errno = ENOMEM;
        return NULL;
    }

    if(newFlags != flags) {
        if(fcntl(fd, F_SETFL, newFlags) < 0) {
            free(file);
            return NULL;
        }
    }

    file->fd = fd;
    file->bufferType = _IOFBF;
    file->position = lseek(fd, 0, SEEK_CUR);

    for(int i = 0; i < OPEN_MAX; i++) {
        if(!_openFiles[i]) _openFiles[i] = file;
    }

    _openFileCount++;
    return file;
}

int fclose(FILE *file) {
    if(!file) {
        errno = EINVAL;
        return EOF;
    }

    if(fflush(file)) return EOF;

    int status;
    if(!file->mmap) status = close(file->fd);
    else {
        status = msync(file->mmap, file->mmapLength, MS_SYNC);
        if(!status) status = munmap(file->mmap, file->mmapLength);
    }

    if(status) return EOF;
    
    for(int i = 0; i < OPEN_MAX; i++) {
        if(_openFiles[i] == file) {
            _openFiles[i] = NULL;
            if((file != stdin) && (file != stdout) && (file != stderr))
                free(file);
            _openFileCount--;
            return 0;
        }
    }

    return EOF;
}

int feof(FILE *file) {
    if(file->eof) return 1;
    if(file->mmap && (file->position >= file->mmapLength)) return 1;

    struct stat st;
    if(fstat(file->fd, &st)) return 1;

    off_t position = lseek(file->fd, 0, SEEK_CUR);
    if(position < 0) {
        file->error = EIO;
        return 0;
    }

    file->eof = (position >= st.st_size);
    return file->eof;
}

size_t fwrite(const void *buffer, size_t size, size_t count, FILE *f) {
    size_t s = size*count;
    if(!s) return 0;

    clearerr(f);

    if(f->mmap) {
        size_t trueCount;
        if((s + f->position) > f->mmapLength) trueCount = f->mmapLength - f->position;
        else trueCount = s;
        memcpy(f->mmap + f->position, buffer, trueCount);

        f->position += trueCount;
        return count;
    }

    if(s + f->bufferSize >= BUFSIZ) {
        if(fflush(f)) return 0;
        ssize_t trueSize = write(f->fd, buffer, s);
        if(trueSize <= 0) {
            f->error = errno;
            return 0;
        }

        return trueSize / size;
    }

    memcpy(f->buffer + f->bufferSize, buffer, s);
    f->bufferSize += s;
    f->error = 0;
    
    // flush if necessary
    switch(f->bufferType) {
    case _IONBF:
        if(fflush(f)) return 0;
        return count;
    
    case _IOFBF:
        if(f->bufferSize >= BUFSIZ) {
            if(fflush(f)) return 0;
        }
        return count;

    case _IOLBF:
        const char *bufstr = (const char *) buffer;
        for(int i = 0; i < s; i++) {
            if(bufstr[i] == '\n') {
                if(fflush(f)) return 0;
            }
        }
        return count;
    }

    return count;
}

size_t fread(void *buffer, size_t size, size_t count, FILE *f) {
    size_t s = size*count;
    if(!s) return 0;

    clearerr(f);

    if(f->mmap) {
        size_t trueCount;
        if((s + f->position) > f->mmapLength) trueCount = f->mmapLength - f->position;
        else trueCount = s;
        memcpy(buffer, f->mmap + f->position, trueCount);

        f->position += trueCount;
        return trueCount / size;
    }

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

int getchar(void) {
    return getc(stdin);
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

char *fgets(char *s, int n, FILE *f) {
    int i = 0, c;
    while((c = fgetc(f)) != EOF) {
        s[i] = c;
        if(c == '\n') {
            s[i+1] = 0;
            return s;
        }

        i++;
        if(i >= (n-1)) {
            s[i] = 0;
            return s;
        }
    }

    if(!i) {
        f->eof = 1;
        return NULL;
    }
    
    s[i] = 0;
    return s;
}

int fseeko(FILE *f, off_t offset, int where) {
    if(f->mmap) {
        off_t old = f->position;
        switch(where) {
        case SEEK_CUR:
            f->position += offset;
            break;
        case SEEK_SET:
            f->position = offset;
            break;
        case SEEK_END:
            f->position = f->mmapLength - offset;
            break;
        default:
            f->error = EINVAL;
            errno = EINVAL;
            return -1;
        }

        if(f->position < 0 || f->position > f->mmapLength) {
            f->position = old;
            f->error = EINVAL;
            return -1;
        }

        return 0;
    }

    if(fflush(f)) return -1;
    off_t s = lseek(f->fd, offset, where);
    if(s < 0) return -1;
    return 0;
}

int fseek(FILE *f, long offset, int where) {
    return fseeko(f, (off_t) offset, where);
}

off_t ftello(FILE *f) {
    if(f->mmap) return f->position;
    return lseek(f->fd, 0, SEEK_CUR);
}

long ftell(FILE *f) {
    return (long) ftello(f);
}

void rewind(FILE *f) {
    fseek(f, 0L, SEEK_SET);
}

int fileno(FILE *f) {
    return f->fd;
}

int ferror(FILE *f) {
    return f->error;
}

void clearerr(FILE *f) {
    f->eof = 0;
    f->error = 0;
}

int fflush(FILE *f) {
    if(!f && !_openFiles) {
        int status = fflush(stdin);
        if(status) return status;
        status = fflush(stdout);
        if(status) return status;
        return fflush(stderr);
    }

    if(!_openFiles) {
        _openFiles = calloc(OPEN_MAX, sizeof(FILE *));
        if(!_openFiles) {
            errno = ENOMEM;
            f->error = errno;
            return EOF;
        }

        _openFiles[0] = stdin;
        _openFiles[1] = stdout;
        _openFiles[2] = stderr;
    }

    if(!f) {
        for(int i = 0; i < OPEN_MAX; i++) {
            if(_openFiles[i]) {
                int status = fflush(_openFiles[i]);
                if(status) return status;
            }
        }

        return 0;
    }

    if(f->mmap) {
        if(msync(f->mmap, f->mmapLength, MS_SYNC | MS_INVALIDATE)) {
            f->error = errno;
            return EOF;
        }

        return 0;
    }

    if(!f->bufferSize) return 0;

    ssize_t s = write(f->fd, f->buffer, f->bufferSize);
    if(s != f->bufferSize) {
        f->error = errno;
        return EOF;
    }

    f->bufferSize = 0;
    memset(f->buffer, 0, BUFSIZ);
    return 0;
}

int setvbuf(FILE *f, char *buf, int type, size_t size) {
    if(type != _IOLBF && type != _IONBF && type != _IOFBF) {
        errno = EINVAL;
        return EOF;
    }

    fflush(f);
    f->bufferType = type;
    return 0;
}

int remove(const char *path) {
    struct stat st;
    if(lstat(path, &st)) return -1;
    if(S_ISDIR(st.st_mode)) return rmdir(path);
    return unlink(path);
}

int rename(const char *old, const char *new) {
    int status = link(old, new);
    if(status) return -1;
    status = unlink(old);
    if(status) {
        unlink(new);
        return -1;
    }

    return 0;
}