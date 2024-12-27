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

    /* these don't actually mean anything because there's no real hardware
     * terminal support, but for completion's sake */
    termios->c_ispeed = B38400;
    termios->c_ospeed = B38400;
    return 0;
}
