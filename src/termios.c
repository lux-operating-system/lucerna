/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <termios.h>

speed_t cfgetispeed(const struct termios *termios) {
    return termios->c_ispeed;
}

speed_t cfgetospeed(const struct termios *termios) {
    return termios->c_ospeed;
}

int cfsetispeed(struct termios *termios, speed_t speed) {
    termios->c_ispeed = speed;
    return 0;
}

int cfsetospeed(struct termios *termios, speed_t speed) {
    termios->c_ospeed = speed;
    return 0;
}
