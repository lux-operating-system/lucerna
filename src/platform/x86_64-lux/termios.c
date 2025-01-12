/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 *
 * Platform-Specific Code for x86_64 lux
 */

#include <sys/ioctl.h>
#include <termios.h>

/* termios commands - these values are specific to the luxOS pty driver */
#define PTY_SET_INPUT           (0x50 | IOCTL_IN_PARAM)
#define PTY_SET_OUTPUT          (0x60 | IOCTL_IN_PARAM)
#define PTY_SET_LOCAL           (0x70 | IOCTL_IN_PARAM)
#define PTY_SET_CONTROL         (0x80 | IOCTL_IN_PARAM)

#define PTY_GET_INPUT           (0x90 | IOCTL_OUT_PARAM)
#define PTY_GET_OUTPUT          (0xA0 | IOCTL_OUT_PARAM)
#define PTY_GET_LOCAL           (0xB0 | IOCTL_OUT_PARAM)
#define PTY_GET_CONTROL         (0xC0 | IOCTL_OUT_PARAM)

#define PTY_EOF                 0x04
#define PTY_EOL                 0xFF
#define PTY_ERASE               0x7F
#define PTY_INTR                0x03
#define PTY_KILL                0x15
#define PTY_MIN                 0x01
#define PTY_QUIT                0x1C
#define PTY_START               0x11
#define PTY_STOP                0x13
#define PTY_SUSP                0x1A
#define PTY_TIME                0x00

int tcsetattr(int fd, int options, const struct termios *termios) {
    if(ioctl(fd, PTY_SET_INPUT, termios->c_iflag) < 0) return -1;
    if(ioctl(fd, PTY_SET_OUTPUT, termios->c_oflag) < 0) return -1;
    if(ioctl(fd, PTY_SET_LOCAL, termios->c_lflag) < 0) return -1;
    if(ioctl(fd, PTY_SET_CONTROL, termios->c_cflag) < 0) return -1;

    return 0;
}

int tcgetattr(int fd, struct termios *termios) {
    unsigned long temp;
    if(ioctl(fd, PTY_GET_INPUT, &temp) < 0) return -1;
    termios->c_iflag = temp;
    if(ioctl(fd, PTY_GET_OUTPUT, &temp) < 0) return -1;
    termios->c_oflag = temp;
    if(ioctl(fd, PTY_GET_LOCAL, &temp) < 0) return -1;
    termios->c_lflag = temp;
    if(ioctl(fd, PTY_GET_CONTROL, &temp) < 0) return -1;
    termios->c_cflag = temp;

    termios->c_cc[VEOF] = PTY_EOF;
    termios->c_cc[VEOL] = PTY_EOL;
    termios->c_cc[VERASE] = PTY_ERASE;
    termios->c_cc[VINTR] = PTY_INTR;
    termios->c_cc[VKILL] = PTY_KILL;
    termios->c_cc[VMIN] = PTY_MIN;
    termios->c_cc[VQUIT] = PTY_QUIT;
    termios->c_cc[VSTART] = PTY_START;
    termios->c_cc[VSTOP] = PTY_STOP;
    termios->c_cc[VSUSP] = PTY_SUSP;
    termios->c_cc[VTIME] = PTY_TIME;

    /* these don't actually mean anything because there's no real hardware
     * terminal support, but for completion's sake */
    termios->c_ispeed = B38400;
    termios->c_ospeed = B38400;
    return 0;
}
