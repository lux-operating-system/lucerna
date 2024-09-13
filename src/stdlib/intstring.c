/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <stdlib.h>
#include <stdint.h>

char *itoa(int n, char *buffer, int radix) {
    return ltoa((long)n, buffer, radix);
}

int atoi(const char *s) {
    return (long)atol(s);
}

char *ltoa(long n, char *buffer, int radix) {
    if(!radix || radix > HEX) return NULL;

    if(!n) {
        buffer[0] = '0';
        buffer[1] = 0;
        return buffer;
    }

    int length = 0;

    uint64_t un = (uint64_t)n;

    while(un) {
        // convert digit by digit and then reverse the string
        uint64_t digit = un % radix;

        if(digit >= 10) {
            buffer[length] = 'a' + digit - 10;
        } else {
            buffer[length] = '0' + digit;
        }

        length++;
        un /= radix;
    }

    buffer[length] = 0;   // null terminator

    // now reverse the string
    if(length >= 2) {
        for(int i = 0; i < length/2; i++) {
            char tmp = buffer[i];
            buffer[i] = buffer[length-i-1];
            buffer[length-i-1] = tmp;
        }
    }

    return buffer;
}

long atol(const char *s) {
    long v = 0;
    int len = 0;

    while(s[len] >= '0' && s[len] <= '9') {
        len++;      // didn't use strlen so we can only account for numerical characters
    }

    if(!len) return 0;
    if(len == 1) return (long)(s[0] - '0');

    long multiplier = 1;
    for(int i = 1; i < len; i++) {
        multiplier *= 10;
    }

    for(int i = 0; i < len; i++) {
        long digit = s[i] - '0';
        v += (digit * multiplier);
        multiplier /= 10;
    }

    return v;
}
