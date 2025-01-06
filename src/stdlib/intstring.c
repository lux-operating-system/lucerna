/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

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
    while(isspace(*s)) s++;

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

long long strtoll(const char *str, char **endptr, int base) {
    while(isspace(*str)) str++;

    if(*str == '-')
        return -1 * strtoll(str+1, endptr, base);
    else if(*str == '+')
        str++;

    if(!base) {
        if(str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) base = 16;
        else if(str[0] == '0') base = 8;
        else if(str[0] >= '1' && str[0] <= '9') base = 10;
        else {
            if(endptr) *endptr = (char *) str;
            errno = EINVAL;
            return 0;
        }
    }

    if(base < 2 || base > 36) {
        if(endptr) *endptr = (char *) str;
        errno = EINVAL;
        return 0;
    }

    if(base == 16) {
        if(str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
            return strtoll(str+2, endptr, base);
    }

    if(base == 10) {
        long long decimal = atoll(str);
        while(isdigit(*str)) str++;
        if(endptr) *endptr = (char *) str;
        return decimal;
    }

    long long number = 0, multiplier = 1;
    int numberLength = 0;

    if(base < 10) {
        while(isdigit(str[numberLength])) {
            numberLength++;
        }
    } else {
        while(isalnum(str[numberLength])) {
            numberLength++;
        }
    }

    if(!numberLength) return 0;

    for(int i = numberLength-1; i >= 0; i--) {
        char digit = str[i];
        if(digit >= 'a' && digit <= 'z') {
            digit -= 'a' + 10;
        } else if(digit >= 'A' && digit <= 'Z') {
            digit -= 'A' + 10;
        } else {
            digit -= '0';
        }

        number += (digit * multiplier);
        multiplier *= base;
    }

    if(endptr) *endptr = (char *) str + numberLength;
    return number;
}

long strtol(const char *str, char **endptr, int base) {
    return (long) strtoll(str, endptr, base);
}
