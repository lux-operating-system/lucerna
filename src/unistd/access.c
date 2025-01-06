/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

int access(const char *path, int amode) {
    uid_t uid = getuid();
    gid_t gid = getgid();

    struct stat st;
    if(stat(path, &st)) return -1;

    if(uid == st.st_uid) {
        if((amode & R_OK) && !(st.st_mode & S_IRUSR)) {
            errno = EACCES;
            return -1;
        }

        if((amode & W_OK) && !(st.st_mode & S_IWUSR)) {
            errno = EACCES;
            return -1;
        }

        if((amode & X_OK) && !(st.st_mode & S_IXUSR)) {
            errno = EACCES;
            return -1;
        }

        return 0;
    } else if(gid == st.st_gid) {
        if((amode & R_OK) && !(st.st_mode & S_IRGRP)) {
            errno = EACCES;
            return -1;
        }

        if((amode & W_OK) && !(st.st_mode & S_IWGRP)) {
            errno = EACCES;
            return -1;
        }

        if((amode & X_OK) && !(st.st_mode & S_IXGRP)) {
            errno = EACCES;
            return -1;
        }

        return 0;
    } else {
        if((amode & R_OK) && !(st.st_mode & S_IROTH)) {
            errno = EACCES;
            return -1;
        }

        if((amode & W_OK) && !(st.st_mode & S_IWOTH)) {
            errno = EACCES;
            return -1;
        }

        if((amode & X_OK) && !(st.st_mode & S_IXOTH)) {
            errno = EACCES;
            return -1;
        }

        return 0;
    }
}