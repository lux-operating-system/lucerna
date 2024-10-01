/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <string.h>
#include <stdint.h>

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