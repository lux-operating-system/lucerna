/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

/* Implementation of ctype.h: Character data manipulation */

#include <ctype.h>

int isalnum(int ch) {
    if(ch >= '0' && ch <= '9') return ch;
    else if(ch >= 'A' && ch <= 'Z') return ch;
    else if(ch >= 'a' && ch <= 'z') return ch;
    else return 0;
}

int isalpha(int ch) {
    if(ch >= 'A' && ch <= 'Z') return ch;
    else if(ch >= 'a' && ch <= 'z') return ch;
    else return 0;
}

int islower(int ch) {
    if(ch >= 'a' && ch <= 'z') return ch;
    else return 0;
}

int isupper(int ch) {
    if(ch >= 'A' && ch <= 'Z') return ch;
    else return 0;
}

int isdigit(int ch) {
    if(ch >= '0' && ch <= '9') return ch;
    else return 0;
}

int isxdigit(int ch) {
    if(ch >= '0' && ch <= '9') return ch;
    else if(ch >= 'A' && ch <= 'F') return ch;
    else if(ch >= 'a' && ch <= 'f') return ch;
    else return 0;
}

int iscntrl(int ch) {
    if(ch >= 0 && ch <= 0x1F) return ch;
    else if(ch == 0x7F) return ch;
    else return 0;
}

int isgraph(int ch) {
    return isalnum(ch) | ispunct(ch);
}

int isspace(int ch) {
    if(ch == ' ' || ch == 0x0C || ch == 0x0A || ch == '\n' || ch == 0x0D ||
    ch == '\t' || ch == 0x0B) return ch;
    else return 0;
}

int isblank(int ch) {
    if(ch == ' ' || ch == '\t') return ch;
    else return 0;
}

int isprint(int ch) {
    return isgraph(ch) | isblank(ch);
}

int ispunct(int ch) {
    switch(ch) {
    case '!':
    case '"':
    case '#':
    case '$':
    case '%':
    case '&':
    case '\'':
    case '(':
    case ')':
    case '*':
    case '+':
    case '-':
    case ',':
    case '.':
    case '/':
    case ':':
    case ';':
    case '<':
    case '=':
    case '>':
    case '?':
    case '@':
    case '[':
    case ']':
    case '\\':
    case '^':
    case '_':
    case '`':
    case '{':
    case '}':
    case '|':
    case '~':
        return ch;
    default:
        return 0;
    }
}

int tolower(int ch) {
    if((ch >= 'A') && (ch <= 'Z')) return ch+32;
    else return ch;
}

int toupper(int ch) {
    if((ch >= 'a') && (ch <= 'z')) return ch-32;
    else return ch;
}
