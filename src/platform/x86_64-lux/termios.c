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

#define PTY_SET_WINSIZE         (0xD0 | IOCTL_IN_PARAM)
#define PTY_GET_WINSIZE         (0xE0 | IOCTL_OUT_PARAM)

#define PTY_SET_NCSS1           (0x110 | IOCTL_IN_PARAM)
#define PTY_SET_NCSS2           (0x120 | IOCTL_IN_PARAM)
#define PTY_GET_NCSS1           (0x130 | IOCTL_OUT_PARAM)
#define PTY_GET_NCSS2           (0x140 | IOCTL_OUT_PARAM)

int tcsetattr(int fd, int options, const struct termios *termios) {
    if(ioctl(fd, PTY_SET_INPUT, termios->c_iflag) < 0) return -1;
    if(ioctl(fd, PTY_SET_OUTPUT, termios->c_oflag) < 0) return -1;
    if(ioctl(fd, PTY_SET_LOCAL, termios->c_lflag) < 0) return -1;
    if(ioctl(fd, PTY_SET_CONTROL, termios->c_cflag) < 0) return -1;

    unsigned long ncss;
    ncss = termios->c_cc[VEOF] & 0xFF;
    ncss |= (termios->c_cc[VEOL] & 0xFF) << 8;
    ncss |= (termios->c_cc[VERASE] & 0xFF) << 16;
    ncss |= (termios->c_cc[VINTR] & 0xFF) << 24;
    ncss |= (termios->c_cc[VKILL] & 0xFF) << 32;
    ncss |= (termios->c_cc[VMIN] & 0xFF) << 40;
    ncss |= (termios->c_cc[VQUIT] & 0xFF) << 48;
    ncss |= (termios->c_cc[VSTART] & 0xFF) << 56;
    if(ioctl(fd, PTY_SET_NCSS1, ncss) < 0) return -1;

    ncss = termios->c_cc[VSTOP] & 0xFF;
    ncss |= (termios->c_cc[VSUSP] & 0xFF) << 8;
    ncss |= (termios->c_cc[VTIME] & 0xFF) << 16;
    return ioctl(fd, PTY_SET_NCSS2, ncss);
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

    if(ioctl(fd, PTY_GET_NCSS1, &temp) < 0) return -1;
    termios->c_cc[VEOF] = temp & 0xFF;
    termios->c_cc[VEOL] = (temp >> 8) & 0xFF;
    termios->c_cc[VERASE] = (temp >> 16) & 0xFF;
    termios->c_cc[VINTR] = (temp >> 24) & 0xFF;
    termios->c_cc[VKILL] = (temp >> 32) & 0xFF;
    termios->c_cc[VMIN] = (temp >> 40) & 0xFF;
    termios->c_cc[VQUIT] = (temp >> 48) & 0xFF;
    termios->c_cc[VSTART] = (temp >> 56) & 0xFF;

    if(ioctl(fd, PTY_GET_NCSS2, &temp) < 0) return -1;
    termios->c_cc[VSTOP] = temp & 0xFF;
    termios->c_cc[VSUSP] = (temp >> 8) & 0xFF;
    termios->c_cc[VTIME] = (temp >> 16) & 0xFF;

    /* these don't actually mean anything because there's no real hardware
     * terminal support, but for completion's sake */
    termios->c_ispeed = B38400;
    termios->c_ospeed = B38400;
    return 0;
}

int tcsetwinsize(int fd, const struct winsize *ws) {
    unsigned long winsize = (ws->ws_col << 16) | ws->ws_row;
    return ioctl(fd, PTY_SET_WINSIZE, winsize);
}

int tcgetwinsize(int fd, struct winsize *ws) {
    unsigned long winsize;
    if(ioctl(fd, PTY_GET_WINSIZE, &winsize) < 0) return -1;
    ws->ws_row = winsize & 0xFFFF;
    ws->ws_col = (winsize >> 16) & 0xFFFF;
    return 0;
}

int tcflush(int fd, int queue) {
    return 0;
}