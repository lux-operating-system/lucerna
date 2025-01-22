/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <signal.h>
#include <stddef.h>

/* This implementation follows the Linux implementation because POSIX is a
 * little vague about the exact step-by-step procedure of abort()
 * https://man7.org/linux/man-pages/man3/abort.3.html
 */

void abort(void) {
    sigset_t sig;
    sigemptyset(&sig);
    sigaddset(&sig, SIGABRT);
    sigprocmask(SIG_UNBLOCK, &sig, NULL);
    raise(SIGABRT);

    signal(SIGABRT, SIG_DFL);
    raise(SIGABRT);
}
