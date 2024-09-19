/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 *
 * Platform-Specific Code for x86_64 lux
 */

/* these functions must be defined for every platform lucerna is to be ported to */

#include <errno.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/lux/lux.h>
#include "syscalls.h"

/* Group 1: Processes, Threads, and Users */

void exit(int status) {
    luxSyscall(SYSCALL_EXIT, status, 0, 0, 0);
}

void _exit(int status) {
    exit(status);
}

pid_t fork(void) {
    return (pid_t) luxSyscall(SYSCALL_FORK, 0, 0, 0, 0);
}

pid_t vfork(void) {
    return fork();
}

int sched_yield(void) {
    return (int) luxSyscall(SYSCALL_YIELD, 0, 0, 0, 0);
}

// TODO: waitpid(), execve()

int execrdv(const char *name, const char **argv) {
    return (int) luxSyscall(SYSCALL_EXECRDV, (uint64_t)name, (uint64_t)argv, 0, 0);
}

pid_t getpid(void) {
    return (pid_t) luxSyscall(SYSCALL_GETPID, 0, 0, 0, 0);
}

pid_t gettid(void) {
    return (pid_t) luxSyscall(SYSCALL_GETTID, 0, 0, 0, 0);
}

uid_t getuid(void) {
    return (uid_t) luxSyscall(SYSCALL_GETUID, 0, 0, 0, 0);
}

gid_t getgid(void) {
    return (gid_t) luxSyscall(SYSCALL_GETGID, 0, 0, 0, 0);
}

// TODO: setuid(), setgid()

unsigned long msleep(unsigned long msec) {
    return (unsigned long) luxSyscall(SYSCALL_MSLEEP, msec, 0, 0, 0);
}

/* Group 2: File System Manipulation */

int open(const char *path, int flags, ...) {
    va_list args;
    va_start(args, flags);
    mode_t mode = va_arg(args, mode_t);
    va_end(args);

    int status = (int) luxSyscall(SYSCALL_OPEN, (uint64_t)path, flags, mode, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return status;
}

int close(int fd) {
    int status = (int) luxSyscall(SYSCALL_CLOSE, fd, 0, 0, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return status;
}

ssize_t read(int fd, void *buffer, size_t count) {
    ssize_t status = (ssize_t) luxSyscall(SYSCALL_READ, fd, (uint64_t) buffer, count, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return status;
}

ssize_t write(int fd, const void *buffer, size_t count) {
    ssize_t status = (ssize_t) luxSyscall(SYSCALL_WRITE, fd, (uint64_t) buffer, count, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return status;
}

int stat(const char *path, struct stat *buf) {
    int status = (int) luxSyscall(SYSCALL_STAT, (uint64_t)path, (uint64_t)buf, 0, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return status;
}

int fstat(int fd, struct stat *buf) {
    int status = (int) luxSyscall(SYSCALL_FSTAT, fd, (uint64_t)buf, 0, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return status;
}

int mount(const char *src, const char *tgt, const char *type, int flags, void *data) {
    int status = (int) luxSyscall(SYSCALL_MOUNT, (uint64_t)src, (uint64_t)tgt, (uint64_t)type, flags);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return status;
}

/* Group 3: Interprocess Communication */

int socket(int domain, int type, int protocol) {
    int status = (int)luxSyscall(SYSCALL_SOCKET, domain, type, protocol, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    } else {
        return status;
    }
}

int connect(int sd, const struct sockaddr *addr, socklen_t len) {
    int status = (int)luxSyscall(SYSCALL_CONNECT, sd, (uint64_t)addr, len, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    } else {
        return status;
    }
}

int bind(int sd, const struct sockaddr *addr, socklen_t len) {
    int status = (int)luxSyscall(SYSCALL_BIND, sd, (uint64_t)addr, len, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    } else {
        return status;
    }
}

int listen(int sd, int backlog) {
    int status = (int)luxSyscall(SYSCALL_LISTEN, sd, backlog, 0, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    } else {
        return status;
    }
}

int accept(int sd, struct sockaddr *addr, socklen_t *len) {
    int status = (int)luxSyscall(SYSCALL_ACCEPT, sd, (uint64_t)addr, (uint64_t)len, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    } else {
        return status;
    }
}

ssize_t recv(int sd, void *buffer, size_t len, int flags) {
    ssize_t status = (ssize_t)luxSyscall(SYSCALL_RECV, sd, (uint64_t)buffer, len, flags);
    if(status < 0) {
        errno = -1*status;
        return -1;
    } else {
        return status;
    }
}

ssize_t send(int sd, const void *buffer, size_t len, int flags) {
    ssize_t status = (ssize_t)luxSyscall(SYSCALL_SEND, sd, (uint64_t)buffer, len, flags);
    if(status < 0) {
        errno = -1*status;
        return -1;
    } else {
        return status;
    }
}

// TODO: kill()

/* Group 4: Memory Management */

void *sbrk(intptr_t delta) {
    return (void *)luxSyscall(SYSCALL_SBRK, (uint64_t)delta, 0, 0, 0);
}

/* Group 5: Driver I/O Functions */

int ioperm(uintptr_t base, uintptr_t count, int enable) {
    int status = (int) luxSyscall(SYSCALL_IOPERM, base, count, enable, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    } else {
        return status;
    }
}

int irq(int pin, IRQHandler *handler) {
    int status = (int) luxSyscall(SYSCALL_IRQ, pin, (uint64_t)handler, 0, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return status;
}