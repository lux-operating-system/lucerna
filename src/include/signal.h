/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#pragma once

#include <sys/types.h>

/* Signal Handler Macros */
#define SIG_DFL         (void (*)(int))0
#define SIG_ERR         (void (*)(int))1
#define SIG_HOLD        (void (*)(int))2
#define SIG_IGN         (void (*)(int))3

#define SIG_T           1   /* terminate */
#define SIG_A           2   /* abort */
#define SIG_C           3   /* continue */
#define SIG_S           4   /* stop */
#define SIG_I           5   /* ignore */

/* ISO C Signals */
#define SIGABRT         1
#define SIGFPE          2
#define SIGILL          3
#define SIGINT          4
#define SIGSEGV         5
#define SIGTERM         6

/* POSIX Extension Signals */
#define SIGALRM         7
#define SIGBUS          8
#define SIGCHLD         9
#define SIGCONT         10
#define SIGHUP          11
#define SIGKILL         12
#define SIGPIPE         13
#define SIGQUIT         14
#define SIGSTOP         15
#define SIGTSTP         16
#define SIGTTIN         17
#define SIGTTOU         18
#define SIGUSR1         19
#define SIGUSR2         20
#define SIGPOLL         21
#define SIGSYS          22
#define SIGTRAP         23
#define SIGURG          24
#define SIGVTALRM       25
#define SIGXCPU         26
#define SIGXFSZ         27

#define MAX_SIGNAL      27

/* Signal Flags */
#define SA_NOCLDSTOP    0x0001
#define SA_ONSTACK      0x0002
#define SA_RESETHAND    0x0004
#define SA_RESTART      0x0008
#define SA_SIGINFO      0x0010
#define SA_NOCLDWAIT    0x0020
#define SA_NODEFER      0x0040

#define SS_ONSTACK      0x0001
#define SS_DISABLE      0x0002

#define MINSIGSTKSZ     4096
#define SIGSTKSZ        16384

/* Signal Masking */
#define SIG_BLOCK       0x0001
#define SIG_UNBLOCK     0x0002
#define SIG_SETMASK     0x0003

typedef volatile uint32_t sig_atomic_t;
typedef uint64_t sigset_t;

typedef struct {
    int si_signo, si_code, si_errno, si_status;
    pid_t si_pid;
    uid_t si_uid;
    void *si_addr;
    long si_band;
} siginfo_t;

struct sigaction {
    union {
        void (*sa_handler)(int);
        void (*sa_sigaction)(int, siginfo_t *, void *);
    };

    sigset_t sa_mask;
    int sa_flags;
};

int sigemptyset(sigset_t *);
int sigfillset(sigset_t *);
int sigaddset(sigset_t *, int);
int sigdelset(sigset_t *, int);
int sigismember(sigset_t *, int);

int kill(pid_t, int);
int raise(int);
void (*signal(int, void (*)(int)))(int);
void (*bsd_signal(int, void (*)(int)))(int);
int sigaction(int, const struct sigaction *, struct sigaction *);
int sigprocmask(int, const sigset_t *, sigset_t *);
