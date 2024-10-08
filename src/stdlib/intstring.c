/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

char *itoa(int n, char *buffer, int radix) {
    return ltoa((long)n, buffer, radix);
}

int atoi(const char *s) {
    return (int) atol(s);
}

char *uitoa(unsigned int n, char *buffer, int radix) {
    return ultoa((unsigned long)n, buffer, radix);
}

char *ltoa(long n, char *buffer, int radix) {
    if(!radix || radix > HEX) return NULL;

    if(!n) {
        buffer[0] = '0';
        buffer[1] = 0;
        return buffer;
    }

    if(n < 0 && radix == DECIMAL) {
        buffer[0] = '-';
        return ltoa(-1 * n, buffer+1, DECIMAL);
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

char *ultoa(unsigned long n, char *buffer, int radix) {
    if(!radix || radix > HEX) return NULL;

    if(!n) {
        buffer[0] = '0';
        buffer[1] = 0;
        return buffer;
    }

    int length = 0;

    while(n) {
        // convert digit by digit and then reverse the string
        unsigned long digit = n % radix;

        if(digit >= 10) {
            buffer[length] = 'a' + digit - 10;
        } else {
            buffer[length] = '0' + digit;
        }

        length++;
        n /= radix;
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
    return (long) atoll(s);
}

long long atoll(const char *s) {
    if(s[0] == '-' && strlen(s) > 1)
        return -1 * atoll(s+1);

    long long v = 0;
    int len = 0;

    while(s[len] >= '0' && s[len] <= '9') {
        len++;      // didn't use strlen so we can only account for numerical characters
    }

    if(!len) return 0;
    if(len == 1) return (long)(s[0] - '0');

    long long multiplier = 1;
    for(int i = 1; i < len; i++) {
        multiplier *= 10;
    }

    for(int i = 0; i < len; i++) {
        long long digit = s[i] - '0';
        v += (digit * multiplier);
        multiplier /= 10;
    }

    return v;
}
