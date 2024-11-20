/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <signal.h>
#include <errno.h>

/* sigemptyset(): clears a set of signals
 * params: set - set of signals
 * returns: zero
 */

int sigemptyset(sigset_t *set) {
    *set = 0;
    return 0;
}

/* sigfillset(): fills a set of signals with all supported signals
 * params: set - set of signals
 * returns: zero
 */

int sigfillset(sigset_t *set) {
    *set = 0;

    for(int i = 0; i < MAX_SIGNAL; i++)
        *set |= (1 << i);

    return 0;
}

/* sigaddset(): adds a signal to a set of signals
 * params: set - set of signals
 * params: signum - signal to add
 * returns: zero on success
 */

int sigaddset(sigset_t *set, int signum) {
    if(signum < 0 || signum > MAX_SIGNAL) {
        errno = EINVAL;
        return -1;
    }

    *set |= (1 << signum);
    return 0;
}

/* sigdelset(): removes a signal from a set of signals
 * params: set - set of signals
 * params: signum - signal to be removed
 * returns: zero on success
 */

int sigdelset(sigset_t *set, int signum) {
    if(signum < 0 || signum > MAX_SIGNAL) {
        errno = EINVAL;
        return -1;
    }

    *set &= ~(1 << signum);
    return 0;
}

/* sigismember(): tests if a signal is in a set of signals
 * params: set - set of signals
 * params: signum - signal to be checked
 * returns: zero if absent, one if present, negative on fail
 */

int sigismember(sigset_t *set, int signum) {
    if(signum < 0 || signum > MAX_SIGNAL) {
        errno = EINVAL;
        return -1;
    }

    if(*set & (1 << signum)) return 1;
    else return 0;
}
