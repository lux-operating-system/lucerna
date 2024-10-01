/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

/* Implementation of unistd.h: Environmental variables */

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// max length of each individual env variable
#define MAX_ENV         8192

char **environ;

static int mod = 0;     // set to 1 after the first call to setenv()
static int envc = 0;    // number of env vars

char *getenv(const char *name) {
    if(!environ) return NULL;

    for(int i = 0; environ[i]; i++) {
        if(!memcmp(name, environ[i], strlen(name))) {
            char *v = environ[i];
            if(v[strlen(name)] == '=') return v + strlen(name) + 1;
        }
    }

    return NULL;
}

char *secure_getenv(const char *name) {
    return getenv(name);
}

static int getenvIndex(const char *name) {
    if(!environ) return -1;

    for(int i = 0; environ[i]; i++) {
        if(!strcmp(name, environ[i])) {
            char *v = environ[i];
            if(v[strlen(name)] == '=') return i;
        }
    }

    return -1;
}

int setenv(const char *name, const char *value, int overwrite) {
    if(!name || !strlen(name)) {
        errno = EINVAL;
        return -1;
    }

    if(!mod) {
        // first time running setenv
        if(environ) {
            for(envc = 0; environ[envc]; envc++);
        }

        // reallocate
        char **newenv = calloc(envc+1, sizeof(char *));
        if(!newenv) {
            errno = ENOMEM;
            return -1;
        }

        // and copy
        if(environ) {
            memcpy(newenv, environ, envc * sizeof(char *));
        }

        environ = newenv;
        environ[envc] = NULL;
    }
    
    // this setup was necessary so that we can realloc() environ when necessary
    // check if the env variable already exists
    int i = getenvIndex(name);
    if(i >= 0) {
        // env variable already exists
        if(!overwrite) return 0;

        // overwrite it
        char *evar = environ[i];
        strcpy(evar + strlen(name) + 1, value);
        return 0;
    }

    // creating a new env var
    envc++;
    char **newenv = realloc(environ, (envc+1) * sizeof(char *));
    if(!newenv) {
        envc--;
        errno = ENOMEM;
        return -1;
    }

    environ = newenv;
    char *nvar = malloc(MAX_ENV);
    if(!nvar) {
        envc--;
        errno = ENOMEM;
        return -1;
    }

    environ[envc-1] = nvar;
    environ[envc] = NULL;

    strcpy(nvar, name);
    strcpy(nvar + strlen(nvar), "=");
    strcpy(nvar + strlen(nvar), value);

    return 0;
}