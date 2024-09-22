/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

/* Linked List Heap Implementation of malloc() and Derivatives */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BRK_INCREMENTS      4096

static void *programBreak = NULL;
static size_t dataSegmentSize = 0;

struct mallocHeader {
    size_t next;           // bytes to skip to next block
    int valid;
};

static size_t roundToBrkIncrement(size_t n) {
    while(n % BRK_INCREMENTS) n++;
    return n;
}

void *malloc(size_t n) {
    if(!n) return NULL;
    void *ptr;
    struct mallocHeader *hdr;
    size_t increment = roundToBrkIncrement(n + sizeof(struct mallocHeader));

    if(!programBreak || !dataSegmentSize) {
        // for the initial allocation
        ptr = sbrk(increment);
        if(ptr == (void *) -1) return NULL;

        programBreak = ptr;
        dataSegmentSize += increment;

        hdr = (struct mallocHeader *) ptr;
        hdr->next = n + sizeof(struct mallocHeader);
        hdr->valid = 1;

        ptr = (void *)((uintptr_t)ptr + sizeof(struct mallocHeader));

        if(hdr->next < (dataSegmentSize - sizeof(struct mallocHeader))) {
            hdr = (struct mallocHeader *)((uintptr_t)hdr + hdr->next);
            hdr->next = 0;
            hdr->valid = 0;
        }
        return ptr;
    }

search:
    // for all other allocations, search for an appropriate free block
    size_t position = 0;
    hdr = (struct mallocHeader *) programBreak;
    while(position < (dataSegmentSize - sizeof(struct mallocHeader))) {
        if(hdr->valid) {
            // used block, skip
            position += hdr->next;
            hdr = (struct mallocHeader *)((uintptr_t)hdr + hdr->next);
        } else {
            // free block, check size
            size_t size;
            if(!hdr->next) size = dataSegmentSize - position - sizeof(struct mallocHeader);
            else size = hdr->next - sizeof(struct mallocHeader);

            if(size >= n) {
                // found a free block
                if(!hdr->next) hdr->next = n + sizeof(struct mallocHeader);

                hdr->valid = 1;
                ptr = (void *)((uintptr_t)hdr + sizeof(struct mallocHeader));
                return ptr;
            } else {
                // free block but not large enough, skip
                position += size + sizeof(struct mallocHeader);
                hdr = (struct mallocHeader *)((uintptr_t)hdr + hdr->next);
            }
        }
    }

    // if we make it here then we didn't find a free block, to try to allocate more memory
    ptr = sbrk(increment);
    if(ptr == (void *)-1) return NULL;  // system is out of memory

    hdr = (struct mallocHeader *) ptr;
    hdr->next = 0;
    hdr->valid = 0;

    dataSegmentSize += increment;
    goto search;
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
