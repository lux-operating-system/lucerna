/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

/* Implementation of printf()-style formatting */

#include <stdarg.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

int vsprintf(char *dst, const char *f, va_list args) {
    int l = 0;
    bool formatter = false;
    char format[16];
    char buffer[40];
    int64_t number;
    int formatIndex;
    int numberLength, paddingLength;
    char paddingCharacter;
    char *str;

    while(*f) {
        if(!formatter && *f == '%') {
            formatter = true;
            formatIndex = 0;
            memset(format, 0, 16);
            f++;
        }

        if(formatter) {
            if(*f == '%') {
                formatter = false;
                if(dst) dst[l] = '%';
                l++;
            } else {
                format[formatIndex] = *f;
                formatIndex++;

                if(*f >= 'A' && *f <= 'z' && *f != 'l' && *f != 'z') {
                    formatter = false;

                    // parse the format
                    memset(buffer, 0, 40);

                    if(format[0] == '.')
                        format[0] = '0';

                    if(format[0] == '0') {
                        paddingCharacter = '0';
                    } else {
                        paddingCharacter = ' ';     // default behavior for some reason
                    }

                    paddingLength = atoi(format);

                    switch(format[formatIndex-1]) {
                    case 'c':
                        number = va_arg(args, int);
                        if(dst) dst[l] = number;
                        l++;
                        break;
                    case 's':
                        str = va_arg(args, char *);
                        if(!str) str = "(null)";
                        if(dst) strcpy(&dst[l], str);
                        l += strlen(str);
                        break;
                    case 'd':
                    case 'i':
                        number = va_arg(args, int64_t);
                        ltoa(number, buffer, DECIMAL);

                        numberLength = strlen(buffer);
                        if(numberLength < paddingLength) {
                            if(number >= 0 || paddingCharacter == ' ') {
                                for(int i = 0; i < (paddingLength-numberLength); i++) {
                                    if(dst) dst[l] = paddingCharacter;
                                    l++;
                                }
                            } else {
                                // negative numbers with a non-whitespace pad
                                memmove(buffer, buffer+1, strlen(buffer));
                                if(dst) dst[l] = '-';
                                l++;

                                for(int i = 0; i < (paddingLength-numberLength); i++) {
                                    if(dst) dst[l] = paddingCharacter;
                                    l++;
                                }
                            }
                        }
                        
                        if(dst) strcpy(&dst[l], buffer);
                        l += numberLength;
                        break;
                    case 'u':
                        number = va_arg(args, int64_t);
                        ultoa(number, buffer, DECIMAL);

                        numberLength = strlen(buffer);
                        if(numberLength < paddingLength) {
                            for(int i = 0; i < (paddingLength-numberLength); i++) {
                                if(dst) dst[l] = paddingCharacter;
                                l++;
                            }
                        }

                        if(dst) strcpy(&dst[l], buffer);
                        l+= numberLength;
                        break;

                    case 'x':
                        number = va_arg(args, uint64_t);
                        ultoa((uint64_t) number, buffer, HEX);

                        numberLength = strlen(buffer);
                        if(numberLength < paddingLength) {
                            for(int i = 0; i < (paddingLength-numberLength); i++) {
                                if(dst) dst[l] = paddingCharacter;
                                l++;
                            }
                        }
                        
                        if(dst) strcpy(&dst[l], buffer);
                        l += numberLength;
                        break;
                    case 'X':
                        number = va_arg(args, uint64_t);
                        ultoa(number, buffer, HEX);

                        numberLength = strlen(buffer);
                        if(numberLength < paddingLength) {
                            for(int i = 0; i < (paddingLength-numberLength); i++) {
                                if(dst) dst[l] = paddingCharacter;
                                l++;
                            }
                        }
                        
                        for(int i = 0; i < numberLength; i++) {
                            if(buffer[i] >= 'a' && buffer[i] <= 'f') {
                                if(dst) dst[l] = buffer[i] - 0x20;
                            } else {
                                if(dst) dst[l] = buffer[i];
                            }

                            l++;
                        }
                        break;
                    
                    case 'p':
                        number = va_arg(args, uintptr_t);
                        ultoa(number, buffer, HEX);

                        if(dst) {
                            dst[l] = '0';
                            dst[l+1] = 'x';
                        }

                        l += 2;

                        for(int i = 0; i < strlen(buffer); i++) {
                            if(buffer[i] >= 'a' && buffer[i] <= 'f') {
                                if(dst) dst[l] = buffer[i] - 0x20;
                            } else {
                                if(dst) dst[l] = buffer[i];
                            }

                            l++;
                        }
                        break;

                    default:
                        if(dst) strcpy(&dst[l], format);
                        l += strlen(format);
                    }
                }
            }
        } else {
            if(dst) dst[l] = *f;
            l++;
        }
        f++;
    }

    if(dst) dst[l] = 0;
    return l;
}

int sprintf(char *dst, const char *f, ...) {
    va_list args;
    va_start(args, f);
    int n = vsprintf(dst, f, args);
    va_end(args);
    return n;
}

int vsnprintf(char *dst, size_t bufsz, const char *f, va_list args) {
    va_list argsCopy;
    va_copy(argsCopy, args);
    int len = vsprintf(NULL, f, args);
    if(!bufsz) return len;
    else if(len > bufsz) {
        char *temp = malloc(len+1);
        if(!temp) {
            errno = ENOMEM;
            return -1;
        }

        vsprintf(temp, f, argsCopy);
        memcpy(dst, temp, bufsz-1);
        dst[bufsz-1] = 0;
        free(temp);

        return len;
    }

    int s = vsprintf(dst, f, argsCopy);
    va_end(argsCopy);
    return s;
}

int snprintf(char *dst, size_t bufsz, const char *f, ...) {
    va_list args;
    va_start(args, f);
    int n = vsnprintf(dst, bufsz, f, args);
    va_end(args);
    return n;
}

int vfprintf(FILE *f, const char *fmt, va_list args) {
    va_list argsCopy;
    va_copy(argsCopy, args);

    int len = vsnprintf(NULL, 0, fmt, args);
    char *temp = malloc(len+1);
    if(!temp) {
        errno = ENOMEM;
        return -1;
    }

    vsnprintf(temp, len, fmt, argsCopy);
    size_t s = fwrite(temp, 1, len, f);
    free(temp);

    va_end(argsCopy);
    return s;
}

int fprintf(FILE *f, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int n = vfprintf(f, fmt, args);
    va_end(args);
    return n;
}

int vprintf(const char *f, va_list args) {
    return vfprintf(stdout, f, args);
}

int printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int n = vprintf(fmt, args);
    va_end(args);
    return n;
}