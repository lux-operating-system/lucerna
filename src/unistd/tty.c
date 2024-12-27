/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <errno.h>
#include <sys/stat.h>

int isatty(int fd) {
    struct stat st;
    if(fstat(fd, &st)) return 0;
    if(S_ISCHR(st.st_mode)) return 1;

    errno = ENOTTY;
    return 0;
}