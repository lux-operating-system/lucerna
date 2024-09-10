/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <string.h>
#include <stdint.h>

/* String Manipulation Functions */

void *memcpy(void *dst, const void *src, size_t n) {
    uint8_t *dstc = (uint8_t *)dst;
    uint8_t *srcc = (uint8_t *)src;

    for(size_t i = 0; i < n; i++) {
        dstc[i] = srcc[i];
    }

    return dst;
}

void *memmove(void *dst, const void *src, size_t n) {
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

void *memset(void *dst, int v, size_t n) {
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

int memcmp(const void *d1, const void *d2, size_t n) {
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