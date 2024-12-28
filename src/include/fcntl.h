/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#pragma once

#include <sys/stat.h>

struct flock {
    short l_type, l_whence;
    off_t l_start, l_len;
    pid_t l_pid;
};

/* open() flags */
#define O_NONBLOCK              0x0001
#define O_NDELAY                O_NONBLOCK
#define O_CLOEXEC               0x0002
#define O_RDONLY                0x0004
#define O_WRONLY                0x0008
#define O_RDWR                  (O_RDONLY | O_WRONLY)
#define O_APPEND                0x0010
#define O_CREAT                 0x0020
#define O_DSYNC                 0x0040
#define O_EXCL                  0x0080
#define O_NOCTTY                0x0100
#define O_RSYNC                 0x0200
#define O_SYNC                  0x0400
#define O_TRUNC                 0x0800
#define O_ACCMODE               (O_RDONLY | O_WRONLY | O_RDWR)

/* fcntl() commands */
#define F_DUPFD                 1
#define F_GETFD                 2
#define F_SETFD                 3
#define F_GETFL                 4
#define F_SETFL                 5
#define F_GETLK                 6
#define F_SETLK                 7
#define F_SETLKW                8
#define F_GETOWN                9
#define F_SETOWN                10

/* fcntl() flags */
#define FD_CLOEXEC              (O_CLOEXEC)

/* file locks */
#define F_UNLOCK                1
#define F_RDLCK                 2
#define F_WRLCK                 4

int open(const char *, int, ...);
int fcntl(int, int, ...);
int creat(const char *, mode_t);
