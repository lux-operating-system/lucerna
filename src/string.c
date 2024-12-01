/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

/* String Manipulation Functions */

void *memcpy(void *dst, const void *src, size_t n) {
    if(!n) return dst;
    uint8_t *dstc = (uint8_t *)dst;
    uint8_t *srcc = (uint8_t *)src;

    for(size_t i = 0; i < n; i++) {
        dstc[i] = srcc[i];
    }

    return dst;
}

void *memmove(void *dst, const void *src, size_t n) {
    if(!n) return dst;

    // force volatile here so it only uses byte-by-byte accesses
    // this is necessary for overlapping memory areas
    uint8_t volatile *dstc = (uint8_t volatile *)dst;
    uint8_t volatile *srcc = (uint8_t volatile *)src;

    for(size_t i = 0; i < n; i++) {
        dstc[i] = srcc[i];
    }

    return dst;
}

size_t strlen(const char *s) {
    size_t i = 0;
    for(; *s; i++) {
        s++;
    }
    return i;
}

char *strcpy(char *dst, const char *src) {
    return (char *)memcpy(dst, src, strlen(src)+1);
}

char *strncpy(char *dst, const char *src, size_t n) {
    size_t len = strlen(src) + 1;
    if(n > len) {
        strcpy(dst, src);
        return (char *) memset(dst + len, 0, n - len);
    } else {
        return (char *) memcpy(dst, src, n);
    }
}

void *memset(void *dst, int v, size_t n) {
    if(!n) return dst;
    uint8_t *dstc = (uint8_t *)dst;
    for(size_t i = 0; i < n; i++) {
        dstc[i] = v;
    }
    return dst;
}

int strcmp(const char *s1, const char *s2) {
    while(*s1 == *s2) {
        if(!*s1) return 0;

        s1++;
        s2++;
    }

    return *s1 - *s2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
    size_t count = 0;

    while(*s1 == *s2) {
        if(!*s1) return 0;

        s1++;
        s2++;
        count++;
        if(count >= n) return 0;
    }

    return *s1 - *s2;
}

int memcmp(const void *d1, const void *d2, size_t n) {
    if(!n) return 0;
    uint8_t *d1c = (uint8_t *)d1;
    uint8_t *d2c = (uint8_t *)d2;

    for(size_t i = 0; i < n; i++) {
        if(*d1c != *d2c) {
            return *d1c - *d2c;
        }

        d1c++;
        d2c++;
    }

    return 0;
}

static int strContainsChar(const char *str, char c) {
    for(int i = 0; i < strlen(str); i++) {
        if(str[i] == c) return 1;
    }

    return 0;
}

static char *strtoklast = NULL;

char *strtok_r(char *s1, const char *s2, char **lasts) {
    if(!s1) s1 = *lasts;
    char *start = NULL;

    // search for a byte that is NOT in s2
    for(int i = 0; i < strlen(s1); i++) {
        if(!strContainsChar(s2, s1[i])) {
            // start of token
            start = &s1[i];

            i++;
            if(!s1[i]) return NULL;

            // now search for the end of the token
            while(s1[i] && !strContainsChar(s2, s1[i])) i++;

            s1[i] = 0;      // null terminate
            *lasts = &s1[i+1];

            return start;
        }
    }

    return NULL;
}

char *strtok(char *s1, const char *s2) {
    return strtok_r(s1, s2, &strtoklast);
}

char *strerror(int error) {
    switch(error) {
    case E2BIG:         return "too many arguments";
    case EACCES:        return "access denied";
    case EADDRINUSE:    return "address in use";
    case EAFNOSUPPORT:  return "address family not supported";
    case EAGAIN:        return "resource busy, retry later";
    case EALREADY:      return "connection already in progress";
    case EBADF:         return "invalid file descriptor";
    case EBADMSG:       return "invalid message";
    case EBUSY:         return "resource busy";
    case ECANCELED:     return "operation canceled";
    case ECHILD:        return "no child processes";
    case ECONNABORTED:  return "connection aborted";
    case ECONNREFUSED:  return "connection refused";
    case ECONNRESET:    return "connection reset";
    case EDEADLK:       return "deadlocked";
    case EDESTADDRREQ:  return "destination address required";
    case EDOM:          return "function argument not in domain";
    case EDQUOT:        return "reserved";
    case EEXIST:        return "file already exists";
    case EFBIG:         return "file is too large";
    case EFAULT:        return "invalid address";
    case EHOSTUNREACH:  return "host is unreachable";
    case EIDRM:         return "identifier removed";
    case EILSEQ:        return "illegal byte sequence";
    case EINPROGRESS:   return "operation in progress";
    case EINTR:         return "interrupted";
    case EINVAL:        return "invalid argument";
    case EIO:           return "I/O error";
    case EISCONN:       return "socket is already connected";
    case EISDIR:        return "is a directory";
    case ENOTDIR:       return "is not a directory";
    case ELOOP:         return "symlink loop";
    case EMFILE:        return "maximum open files for the process";
    case ENFILE:        return "maximum open files for the system";
    case EMLINK:        return "too many symlinks";
    case EMSGSIZE:      return "message is too large";
    case EMULTIHOP:     return "reserved";
    case ENAMETOOLONG:  return "file name is too long";
    case ENETDOWN:      return "network is down";
    case ENETRESET:     return "connection aborted by network";
    case ENETUNREACH:   return "network is unreachable";
    case ENOBUFS:       return "no free buffers";
    case ENODEV:        return "device doesn't exist";
    case ENOENT:        return "file doesn't exist";
    case ENOEXEC:       return "executable format error";
    case ENOLCK:        return "no free locks";
    case ENOLINK:       return "reserved";
    case ENOMEM:        return "no free memory";
    case ENOMSG:        return "no message of desired type";
    case ENOPROTOOPT:   return "protocol not implemented";
    case ENOSPC:        return "no free storage";
    case ENOSYS:        return "function is not implemented";
    case ENOTCONN:      return "socket is not connected";
    case ENOTEMPTY:     return "directory is not empty";
    case ENOTRECOVERABLE:
        return "unrecoverable error";
    case ENOTSOCK:      return "not a socket";
    case ENOTSUP:       return "operation not supported";
    case ENOTTY:        return "inappropriate I/O control operation";
    case ENXIO:         return "no such I/O device or address";
    case EOVERFLOW:     return "overflow";
    case EPERM:         return "permission denied";
    case EPIPE:         return "broken pipe";
    case EPROTO:        return "protocol error";
    case EPROTONOSUPPORT:
        return "protocol not supported";
    case EPROTOTYPE:    return "invalid protocol type";
    case ERANGE:        return "result is out of range";
    case EROFS:         return "read-only file system";
    case ESPIPE:        return "invalid seek";
    case ESRCH:         return "process doesn't exist";
    case ESTALE:        return "reserved";
    case ETIMEDOUT:     return "connection timeout";
    case ETXTBSY:       return "text file is busy";
    case EWOULDBLOCK:   return "operation is blocked";
    case EXDEV:         return "cross-device link";
    default:
        errno = EINVAL;
        return "undefined error code";
    }
}

char *strchr(const char *s, int c) {
    while(*s) {
        if(*s == c) return (char *) s;
        s++;
    }

    return NULL;
}

char *strrchr(const char *s, int c) {
    size_t len = strlen(s);
    const char *ptr = s + len;

    while(len) {
        if(*ptr == c) return (char *) ptr;
        len--;
        ptr--;
    }

    return NULL;
}

char *strdup(const char *s) {
    char *newptr = malloc(strlen(s) + 1);
    if(!newptr) {
        errno = ENOMEM;
        return NULL;
    }

    return strcpy(newptr, s);
}

char *strstr(const char *s1, const char *s2) {
    // search for s2 in s1
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);

    if(!len2) return (char *) s1;

    for(size_t i = 0; i < len1-len2+1; i++) {
        if(!memcmp(s1+i, s2, len2)) return (char *) s1 + i;
    }

    return NULL;
}