/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 *
 * Platform-Specific Code for x86_64 lux
 */

/* these functions must be defined for every platform lucerna is to be ported to */

#include <errno.h>
#include <stdarg.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <utime.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/statvfs.h>
#include <sys/lux/lux.h>
#include "syscalls.h"

/* Group 1: Processes, Threads, and Users */

void _exit(int status) {
    luxSyscall(SYSCALL_EXIT, status, 0, 0, 0);
}

pid_t fork(void) {
    pid_t status = (pid_t) luxSyscall(SYSCALL_FORK, 0, 0, 0, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return status;
}

pid_t vfork(void) {
    return fork();
}

int sched_yield(void) {
    return (int) luxSyscall(SYSCALL_YIELD, 0, 0, 0, 0);
}

pid_t waitpid(pid_t pid, int *status, int options) {
    pid_t ret = (pid_t) luxSyscall(SYSCALL_WAITPID, pid, (uint64_t) status, options, 0);
    if(ret < 0) {
        errno = -1*ret;
        return -1;
    }

    return ret;
}

int execve(const char *path, char *const argv[], char *const envp[]) {
    int status = (int) luxSyscall(SYSCALL_EXECVE, (uint64_t)path, (uint64_t)argv, (uint64_t)envp, 0);

    // execve() doesn't return unless an error occurred
    errno = -1*status;
    return -1;
}

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

int usleep(useconds_t useconds) {
    return (int) luxSyscall(SYSCALL_MSLEEP, useconds / 1000, 0, 0, 0);
}

int gettimeofday(struct timeval *tz, void *tzp) {
    return (int) luxSyscall(SYSCALL_GETTIMEOFDAY, (uint64_t) tz, (uint64_t) tzp, 0, 0);
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

int lstat(const char *path, struct stat *buf) {
    int status = (int) luxSyscall(SYSCALL_LSTAT, (uint64_t)path, (uint64_t)buf, 0, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return status;
}

int stat(const char *path, struct stat *buf) {
    if(lstat(path, buf)) return -1;
    if(S_ISLNK(buf->st_mode)) {
        char linkTarget[PATH_MAX+1];
        ssize_t s = readlink(path, linkTarget, PATH_MAX);
        if(s < 0) return -1;
        linkTarget[s] = 0;
        return stat(linkTarget, buf);
    }

    return 0;
}

int fstat(int fd, struct stat *buf) {
    int status = (int) luxSyscall(SYSCALL_FSTAT, fd, (uint64_t)buf, 0, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return status;
}

off_t lseek(int fd, off_t offset, int where) {
    if(where == SEEK_END) {
        // this will allow SEEK_END without explicitly giving the kernel the
        // sizes of all open files - this also helps keep the kernel code to
        // an absolute minimum where possible

        struct stat st;
        if(fstat(fd, &st)) return -1;

        off_t n = st.st_size;
        n += offset;
        return lseek(fd, n, SEEK_SET);
    }

    off_t status = (off_t) luxSyscall(SYSCALL_LSEEK, fd, offset, where, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return status;
}

int chown(const char *path, uid_t owner, gid_t group) {
    int status = (int) luxSyscall(SYSCALL_CHOWN, (uint64_t) path, owner, group, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return 0;
}

int chmod(const char *path, mode_t mode) {
    int status = (int) luxSyscall(SYSCALL_CHMOD, (uint64_t) path, mode, 0, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return 0;
}

int link(const char *old, const char *new) {
    int status = (int) luxSyscall(SYSCALL_LINK, (uint64_t) old, (uint64_t) new, 0, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return 0;
}

int unlink(const char *path) {
    int status = (int) luxSyscall(SYSCALL_UNLINK, (uint64_t) path, 0, 0, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return 0;
}

int symlink(const char *old, const char *new) {
    int status = (int) luxSyscall(SYSCALL_SYMLINK, (uint64_t) old, (uint64_t) new, 0, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return 0;
}

ssize_t readlink(const char *path, char *buf, size_t bufsiz) {
    ssize_t status = (ssize_t) luxSyscall(SYSCALL_READLINK, (uint64_t) path, (uint64_t) buf, bufsiz, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return status;
}

mode_t umask(mode_t cmask) {
    return (mode_t) luxSyscall(SYSCALL_UMASK, cmask, 0, 0, 0);
}

int mkdir(const char *path, mode_t mode) {
    int status = (int) luxSyscall(SYSCALL_MKDIR, (uint64_t) path, mode, 0, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return 0;
}

int utime(const char *path, const struct utimbuf *times) {
    int status = (int) luxSyscall(SYSCALL_UTIME, (uint64_t) path, (uint64_t) times, 0, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return 0;
}

int chdir(const char *path) {
    int status = (int) luxSyscall(SYSCALL_CHDIR, (uint64_t)path, 0, 0, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return status;
}

char *getcwd(char *buf, size_t bufsz) {
    intptr_t ret = (intptr_t) luxSyscall(SYSCALL_GETCWD, (uint64_t)buf, bufsz, 0, 0);
    if(ret < 0) {
        errno = -1*ret;
        return NULL;
    }

    return (char *) ret;
}

int mount(const char *src, const char *tgt, const char *type, int flags, void *data) {
    int status = (int) luxSyscall(SYSCALL_MOUNT, (uint64_t)src, (uint64_t)tgt, (uint64_t)type, flags);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return status;
}

int umount2(const char *tgt, int flags) {
    int status = (int) luxSyscall(SYSCALL_UMOUNT2, (uint64_t) tgt, flags, 0, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return status;
}

int umount(const char *tgt) {
    return umount2(tgt, 0);
}

int fcntl(int fd, int cmd, ...) {
    va_list args;
    va_start(args, cmd);
    uintptr_t next = va_arg(args, uintptr_t);

    int status = (int) luxSyscall(SYSCALL_FCNTL, fd, cmd, next, 0);
    if(status < 0) {
        errno = -1*status;
        status = -1;
    }

    va_end(args);
    return status;
}

DIR *opendir(const char *path) {
    long status = (long) luxSyscall(SYSCALL_OPENDIR, (uint64_t)path, 0, 0, 0);
    if(status < 0) {
        errno = -1*status;
        return NULL;
    }

    return (DIR *) status;
}

int closedir(DIR *dir) {
    int status = (int) luxSyscall(SYSCALL_CLOSEDIR, (uint64_t)dir, 0, 0, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return 0;
}

int readdir_r(DIR *dir, struct dirent *entry, struct dirent **result) {
    int status = (int) luxSyscall(SYSCALL_READDIR_R, (uint64_t) dir, (uint64_t) entry, (uint64_t) result, 0);
    if(status < 0) {
        errno = -1*status;
        return errno;
    }

    return 0;
}

static struct dirent *direntry = NULL;  /* for the non-thread-safe readdir() */

struct dirent *readdir(DIR *dir) {
    if(!direntry) direntry = malloc(sizeof(struct dirent) + PATH_MAX);
    if(!direntry) {
        errno = ENOMEM;
        return NULL;
    }

    struct dirent **ptr = &direntry;
    if(!readdir_r(dir, direntry, ptr)) return *ptr;
    else return NULL;
}

void seekdir(DIR *dir, long position) {
    luxSyscall(SYSCALL_SEEKDIR, (uint64_t) dir, position, 0, 0);
}

long telldir(DIR *dir) {
    return (long) luxSyscall(SYSCALL_TELLDIR, (uint64_t) dir, 0, 0, 0);
}

void rewinddir(DIR *dir) {
    seekdir(dir, 0);
}

int fsync(int fd) {
    int status = (int) luxSyscall(SYSCALL_FSYNC, fd, 0, 0, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return 0;
}

int statvfs(const char *path, struct statvfs *buf) {
    int status = (int) luxSyscall(SYSCALL_STATVFS, (uint64_t) path, (uint64_t) buf, 0, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return 0;
}

int fstatvfs(int fd, struct statvfs *buf) {
    int status = (int) luxSyscall(SYSCALL_FSTATVFS, fd, (uint64_t) buf, 0, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return 0;
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

int kill(pid_t pid, int signum) {
    int status = (int) luxSyscall(SYSCALL_KILL, pid, signum, 0, 0);
    if(status) {
        errno = -1*status;
        return -1;
    }

    return 0;
}

int sigaction(int sig, const struct sigaction *act, struct sigaction *oact) {
    int status = (int) luxSyscall(SYSCALL_SIGACTION, sig, (uint64_t) act, (uint64_t) oact, 0);
    if(status) {
        errno = -1*status;
        return -1;
    }

    return 0;
}

void (*signal(int sig, void (*func)(int)))(int) {
    struct sigaction act, oact;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = func;

    int status = sigaction(sig, &act, &oact);

    if(status) return SIG_ERR;
    else return oact.sa_handler;
}

/* Group 4: Memory Management */

struct MmapSyscallParams {
    // probably the only syscall whose params will be passed in memory because
    // there's just too many
    void *addr;
    size_t len;
    int prot;
    int flags;
    int fd;
    off_t off;
};

void *sbrk(intptr_t delta) {
    return (void *)luxSyscall(SYSCALL_SBRK, (uint64_t)delta, 0, 0, 0);
}

void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off) {
    struct MmapSyscallParams p;
    p.addr = addr;
    p.len = len;
    p.prot = prot;
    p.flags = flags;
    p.fd = fd;
    p.off = off;

    intptr_t ptr = (intptr_t) luxSyscall(SYSCALL_MMAP, (uint64_t) &p, 0, 0, 0);
    if(ptr < 0) {
        errno = -1 * ptr;
        return MAP_FAILED;
    }

    return (void *) ptr;
}

int munmap(void *addr, size_t len) {
    int status = (int) luxSyscall(SYSCALL_MUNMAP, (uint64_t) addr, len, 0, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return 0;
}

int msync(void *addr, size_t len, int flags) {
    int status = (int) luxSyscall(SYSCALL_MSYNC, (uint64_t) addr, len, flags, 0);
    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return 0;
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

int ioctl(int fd, unsigned long op, ...) {
    va_list args;
    va_start(args, op);

    int status;
    if(op & IOCTL_IN_PARAM) {
        unsigned long param = va_arg(args, unsigned long);
        status = (int) luxSyscall(SYSCALL_IOCTL, fd, op, param, 0);
    } else if(op & IOCTL_OUT_PARAM) {
        unsigned long *param = va_arg(args, unsigned long *);
        status = (int) luxSyscall(SYSCALL_IOCTL, fd, op, (uint64_t) param, 0);
    } else {
        status = (int) luxSyscall(SYSCALL_IOCTL, fd, op, 0, 0);
    }

    va_end(args);

    if(status < 0) {
        errno = -1*status;
        return -1;
    }

    return status;
}

uintptr_t mmio(uintptr_t addr, off_t count, int flags) {
    return (uintptr_t) luxSyscall(SYSCALL_MMIO, addr, count, flags, 0);
}

uintptr_t pcontig(uintptr_t addr, off_t count, int flags) {
    return (uintptr_t) luxSyscall(SYSCALL_PCONTIG, addr, count, flags, 0);
}

uintptr_t vtop(uintptr_t addr) {
    return (uintptr_t) luxSyscall(SYSCALL_VTOP, addr, 0, 0, 0);
}