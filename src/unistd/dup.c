/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int dup(int fd) {
    return fcntl(fd, F_DUPFD, 0);
}

int dup2(int fd, int fd2) {
    if(fd == fd2) return fd2;
    int status = fcntl(fd2, F_GETFD);
    if(status >= 0) {
        if(close(fd2)) return -1;
    }

    int new = fcntl(fd, F_DUPFD, fd2);
    if(new < 0) return -1;
    if(new != fd2) {
        close(new);
        errno = EMFILE;
        return -1;
    }

    return new;
}

int dup3(int fd, int fd2, int flags) {
    if(fd == fd2) {
        errno = EINVAL;
        return -1;
    }

    int status = fcntl(fd2, F_GETFD);
    if(status >= 0) {
        if(close(fd2)) return -1;
    }

    int new = fcntl(fd, F_DUPFD, fd2);
    if(new < 0) return -1;
    if(new != fd2) {
        close(new);
        errno = EMFILE;
        return -1;
    }

    int fcntlFlags = 0;
    if(flags & O_CLOEXEC) fcntlFlags |= FD_CLOEXEC;
    if(flags & O_CLOFORK) fcntlFlags |= FD_CLOFORK;

    if(fcntlFlags) {
        if(fcntl(new, F_SETFD, fcntlFlags) < 0) {
            close(new);
            return -1;
        }
    }

    return new;
}