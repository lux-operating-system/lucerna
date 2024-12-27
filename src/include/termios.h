/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#pragma once

#include <sys/types.h>      /* pid_t */

#define NCSS                11

typedef unsigned long cc_t;
typedef unsigned long speed_t;
typedef unsigned long tcflag_t;

struct termios {
    tcflag_t c_iflag, c_oflag, c_cflag, c_lflag;
    speed_t c_ispeed, c_ospeed;
    cc_t c_cc[NCSS];
};

/* c_cc indexes */
#define VEOF                0
#define VEOL                1
#define VERASE              2
#define VINTR               3
#define VKILL               4
#define VMIN                5
#define VQUIT               6
#define VSTART              7
#define VSTOP               8
#define VSUSP               9
#define VTIME               10

/* c_iflag */
#define BRKINT              0x0001
#define ICRNL               0x0002
#define IGNBRK              0x0004
#define IGNCR               0x0008
#define IGNPAR              0x0010
#define INLCR               0x0020
#define INPCK               0x0040
#define ISTRIP              0x0080
#define IXANY               0x0100
#define IXOFF               0x0200
#define IXON                0x0400
#define PARMRK              0x0800

/* c_oflag */
#define OPOST               0x0001
#define ONLCR               0x0002
#define OCRNL               0x0004
#define ONOCR               0x0008
#define ONLRET              0x0010
#define OFILL               0x0020
#define NLDLY               0x0040
#define NL0                 0x0000
#define NL1                 0x0040
#define CRDLY               0x0180
#define CR0                 0x0000
#define CR1                 0x0080
#define CR2                 0x0100
#define CR3                 0x0180
#define TABDLY              0x0600
#define TAB0                0x0000
#define TAB1                0x0200
#define TAB2                0x0400
#define TAB3                0x0600
#define BSDLY               0x0800
#define BS0                 0x0000
#define BS1                 0x0800
#define VTDLY               0x1000
#define VT0                 0x0000
#define VT1                 0x1000
#define FFDLY               0x2000
#define FF0                 0x0000
#define FF1                 0x2000

/* c_cflag */
#define CSIZE               0x0003
#define CS5                 0x0000
#define CS6                 0x0001
#define CS7                 0x0002
#define CS8                 0x0003
#define CSTOPB              0x0004
#define CREAD               0x0008
#define PARENB              0x0010
#define PARODD              0x0020
#define HUPCL               0x0040
#define CLOCAL              0x0080

/* c_lflag */
#define ECHO                0x0001
#define ECHOE               0x0002
#define ECHOK               0x0004
#define ECHONL              0x0008
#define ICANON              0x0010
#define IEXTEN              0x0020
#define ISIG                0x0040
#define NOFLSH              0x0080
#define TOSTOP              0x0100

/* baud rates for speed_t */
#define B0                  0
#define B50                 50
#define B75                 75
#define B110                110
#define B134                134
#define B150                150
#define B200                200
#define B300                300
#define B600                600
#define B1200               1200
#define B1800               1800
#define B2400               2400
#define B4800               4800
#define B9600               9600
#define B19200              19200
#define B38400              38400

/* tcsetattr() flags */
#define TCSANOW             1
#define TCSADRAIN           2
#define TCSAFLUSH           3

/* tcflush() flags */
#define TCIFLUSH            0x01
#define TCOFLUSH            0x02
#define TCIOFLUSH           (TCIFLUSH | TCOFLUSH)

/* tcflow() flags */
#define TCIOFF              1
#define TCION               2
#define TCOOFF              3
#define TCOON               4

speed_t cfgetispeed(const struct termios *);
speed_t cfgetospeed(const struct termios *);
int cfsetispeed(struct termios *, speed_t);
int cfsetospeed(struct termios *, speed_t);
int tcdrain(int);
int tcflow(int, int);
int tcflush(int, int);
int tcgetattr(int, struct termios *);
int tcsetattr(int, int, const struct termios *);
pid_t tcgetsid(int);
int tcsendbreak(int, int);
