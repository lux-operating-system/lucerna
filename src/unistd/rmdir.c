/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

int rmdir(const char *path) {
    struct stat st;
    if(lstat(path, &st)) return -1;
    if(!S_ISDIR(st.st_mode)) {
        errno = ENOTDIR;
        return -1;
    }

    return unlink(path);
}
