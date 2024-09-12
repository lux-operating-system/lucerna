/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

/* Linked List Heap Implementation of malloc() and Derivatives */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BRK_INCREMENTS      65536       // so we don't have to keep making syscalls on every allocation

struct mallocHeader {
    size_t next;           // bytes to skip to next block
    int valid;
};

static void *heapStart = NULL, *heapEnd = NULL;
static size_t heapSize = 0;

static size_t roundToBrkIncrement(size_t n) {
    while(n % BRK_INCREMENTS) n++;
    return n;
}

void *malloc(size_t n) {
    if(!n) return NULL;
    struct mallocHeader *hdr;

    n = roundToBrkIncrement(n);
    void *brk = sbrk(n + sizeof(struct mallocHeader));
    if(!brk || brk == (void *)-1) return NULL;

    hdr = brk;
    hdr->valid = 1;
    hdr->next = n;

    return (void *)((uintptr_t)brk + sizeof(struct mallocHeader));
}

void free(void *ptr) {
    if(!ptr) return;

    struct mallocHeader *h = (struct mallocHeader *)((uintptr_t)ptr - sizeof(struct mallocHeader));
    h->valid = 0;
}

void *calloc(size_t num, size_t size) {
    size_t s = num*size;
    if(!s) return NULL;

    void *ptr = malloc(s);
    if(!ptr) return NULL;

    return memset(ptr, 0, s);
}

void *realloc(void *ptr, size_t newSize) {
    if(!newSize) return NULL;
    if(!ptr) return malloc(newSize);

    void *newPtr = malloc(newSize);
    if(!newPtr) return NULL;

    struct mallocHeader *h = (struct mallocHeader *)((uintptr_t)ptr - sizeof(struct mallocHeader));
    size_t oldSize = h->next - sizeof(struct mallocHeader);

    if(newSize >= oldSize) {
        memcpy(newPtr, ptr, oldSize);
    } else {
        memcpy(newPtr, ptr, newSize);
    }

    free(ptr);
    return newPtr;
}
