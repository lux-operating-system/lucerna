/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 *
 * Platform-Specific Code for x86_64 lux
 */

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>

/* ioctl commands here are specific to the luxOS pty driver and will not work
 * on any other terminal implementation */

#define PTY_GET_SLAVE           (0x10 | IOCTL_OUT_PARAM)

static char ptyname[128];

/* ptsname(): returns the name of the slave pseudo-terminal
 * params: fd - file descriptor of the master
 * returns: pointer to the slave file name, NULL on error
 */

char *ptsname(int fd) {
    unsigned long id;
    if(!ioctl(fd, PTY_GET_SLAVE, &id)) {
        strcpy(ptyname, "/dev/pts");
        ltoa(id, &ptyname[8], DECIMAL);

        return ptyname;
    } else {
        return NULL;        // errno is already set by ioctl()
    }
}

/* ptsname_r(): returns the name of the slave pseudo-terminal in a buffer
 * note: this function is NOT in the POSIX standard, but it is implemented by
 * Linux and OpenBSD and frankly it is quite useful and more secure
 * 
 * params: fd - file descriptor of the master
 * params: buf - destination buffer
 * params: bufsz - buffer size
 * returns: zero on success, error code on error
 */

int ptsname_r(int fd, char *buf, size_t bufsz) {
    char *pts = ptsname(fd);
    if(!pts) return errno;

    if(strlen(pts) > bufsz) {
        memcpy(buf, pts, bufsz-1);
        buf[bufsz-1] = 0;
    } else {
        strcpy(buf, pts);
    }

    return 0;
}