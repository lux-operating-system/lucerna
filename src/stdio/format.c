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

int vsprintf(char *dst, const char *f, va_list args) {
    int l = 0;
    bool formatter = false;
    char format[16];
    char buffer[40];
    uint64_t number;
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
                dst[l] = '%';
                l++;
            } else {
                format[formatIndex] = *f;
                formatIndex++;

                if(*f >= 'A' && *f <= 'z') {
                    formatter = false;

                    // parse the format
                    memset(buffer, 0, 40);

                    if(format[0] == '0') {
                        paddingCharacter = '0';
                    } else {
                        paddingCharacter = ' ';     // default behavior for some reason
                    }

                    paddingLength = atoi(format);

                    switch(format[formatIndex-1]) {
                    case 'c':
                        number = va_arg(args, int);
                        dst[l] = number;
                        l++;
                        break;
                    case 's':
                        str = va_arg(args, char *);
                        strcpy(&dst[l], str);
                        l += strlen(str);
                        break;
                    case 'd':
                    case 'i':
                    case 'u':
                        number = va_arg(args, uint64_t);
                        ltoa(number, buffer, DECIMAL);

                        numberLength = strlen(buffer);
                        if(numberLength < paddingLength) {
                            for(int i = 0; i < (paddingLength-numberLength); i++) {
                                dst[l] = paddingCharacter;
                                l++;
                            }
                        }
                        
                        strcpy(&dst[l], buffer);
                        l += numberLength;
                        break;
                    case 'x':
                        number = va_arg(args, uint64_t);
                        ltoa(number, buffer, HEX);

                        numberLength = strlen(buffer);
                        if(numberLength < paddingLength) {
                            for(int i = 0; i < (paddingLength-numberLength); i++) {
                                dst[l] = paddingCharacter;
                                l++;
                            }
                        }
                        
                        strcpy(&dst[l], buffer);
                        l += numberLength;
                        break;
                    case 'X':
                        number = va_arg(args, uint64_t);
                        ltoa(number, buffer, HEX);

                        numberLength = strlen(buffer);
                        if(numberLength < paddingLength) {
                            for(int i = 0; i < (paddingLength-numberLength); i++) {
                                dst[l] = paddingCharacter;
                                l++;
                            }
                        }
                        
                        for(int i = 0; i < numberLength; i++) {
                            if(buffer[i] >= 'a' && buffer[i] <= 'f') {
                                dst[l] = buffer[i] - 0x20;
                            } else {
                                dst[l] = buffer[i];
                            }

                            l++;
                        }
                        break;
                    default:
                        strcpy(&dst[l], format);
                        l += strlen(format);
                    }
                }
            }
        } else {
            dst[l] = *f;
            l++;
        }
        f++;
    }

    dst[l] = 0;
    return l;
}

int sprintf(char *dst, const char *f, ...) {
    va_list args;
    va_start(args, f);
    int n = vsprintf(dst, f, args);
    va_end(args);
    return n;
}
