/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#pragma once

#include <sys/types.h>

typedef int DIR;

struct dirent {
    ino_t d_ino;
    char d_name[];
};

DIR *opendir(const char *);
int closedir(DIR *);
struct dirent *readdir(DIR *);
int readdir_r(DIR *, struct dirent *, struct dirent **);
void rewinddir(DIR *);
void seekdir(DIR *, long);
long telldir(DIR *);
