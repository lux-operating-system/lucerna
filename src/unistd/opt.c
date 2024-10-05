/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

/* Implementation of unistd.h: Command-line argument parser */

#include <unistd.h>
#include <string.h>
#include <stdio.h>

char *optarg = NULL;
int optind = 1, opterr = 1;
int optopt = 0;

// internally used to track multiple flags within the same argument; POSIX says
// the way we do this is implementation-specific - this is also not thread-safe
// but getopt() itself is not a thread-safe function

static int argindex = 1;

int getopt(int argc, char *const argv[], const char *optstr) {
    if(optind >= argc) return -1;
    if(!argv || !argv[optind] || *argv[optind] != '-' || !strcmp(argv[optind], "-")) return -1;
    if(!strcmp(argv[optind], "--")) {
        optind++;
        return -1;
    }

    size_t arglen = strlen(argv[optind]);
    char c = argv[optind][argindex];
    argindex++;

    // check if this character is in the option string
    int i = 0;
    for(i = 0; i < strlen(optstr); i++) {
        if(optstr[i] == c) {
            if(optstr[i+1] == ':') {
                // option requires an argument
                optopt = c;

                if(argv[optind][argindex]) {
                    // option is present in the same arg
                    optarg = &argv[optind][argindex];
                    optind++;
                    argindex = 1;

                    return c;
                }

                optind++;
                argindex = 1;
                if(optind >= argc) {
                    // missing argument
                    optarg = NULL;
                    
                    if(opterr && (*optstr != ':'))
                        fprintf(stderr, "%s: option requires an argument -- %c\n", argv[0], c);

                    if(*optstr == ':') return ':';
                    else return '?';
                } else {
                    // next argument is present
                    optarg = argv[optind];
                    optind++;
                    return c;
                }
            } else {
                // option does not require an argument
                if(argindex >= arglen) {
                    argindex = 1;
                    optind++;
                }
                return c;
            }
        }
    }

    // option is not present in optstr
    if(argindex >= arglen) {
        argindex = 1;
        optind++;
    }

    return '?';
}
