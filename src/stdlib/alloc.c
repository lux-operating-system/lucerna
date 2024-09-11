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

    if(!heapStart || heapStart == (void *)-1) {
        heapStart = sbrk(BRK_INCREMENTS);
        if(heapStart == (void *)-1) return NULL;
        heapSize = BRK_INCREMENTS;
        heapEnd = heapStart + heapSize;

        hdr = heapStart;
        hdr->next = 0;
        hdr->valid = 0;
    }

    // possibly allocate more memory
    if(n > heapSize || n > BRK_INCREMENTS) {
        size_t inc = roundToBrkIncrement(n);
        void *brk = sbrk(inc);
        if(brk == (void *)-1) return NULL;

        heapSize += inc;
        heapEnd = heapStart + heapSize;

        hdr = brk;
        hdr->next = 0;
        hdr->valid = 0;
    }

    // iterate through the list and look for a block that's large enough
    size_t position = 0;
    hdr = heapStart;
    while(position < heapSize) {
        if(hdr->valid) {
            position += hdr->next;
            hdr = (struct mallocHeader *)((uintptr_t)hdr + hdr->next);
        } else {
            // free block
            size_t size;
            if(!hdr->next) size = heapSize - position - sizeof(struct mallocHeader);
            else size = hdr->next - sizeof(struct mallocHeader);

            if(size >= n) {
                // appropriate size
                hdr->next = n + sizeof(struct mallocHeader);
                hdr->valid = 1;

                void *ptr = (void *)(uintptr_t)hdr + sizeof(struct mallocHeader);

                hdr = (struct mallocHeader *)((uintptr_t)hdr + hdr->next);
                if(!hdr->valid) hdr->next = 0;

                return ptr;
            }

            position += size + sizeof(struct mallocHeader);
        }
    }

    return NULL;
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