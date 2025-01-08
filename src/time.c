/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <time.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/time.h>

static int __daysPerMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

time_t time(time_t *tloc) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    if(tloc) *tloc = tv.tv_sec;
    return tv.tv_sec;
}

time_t mktime(struct tm *timeptr) {
    time_t timer;

    int year = timeptr->tm_year + 1900;
    int leap = (!(year % 4) && (year % 100)) || (!(year % 400));

    timeptr->tm_yday = 0;
    timeptr->tm_wday = 0;

    for(int i = 0; i < timeptr->tm_mon; i++) {
        if(i == 2 && leap) timeptr->tm_yday += 29;   // special case for february
        else timeptr->tm_yday += __daysPerMonth[i-1];
    }

    timeptr->tm_yday += timeptr->tm_mday - 1;

    timer = timeptr->tm_sec + (timeptr->tm_min*60) + (timeptr->tm_hour*3600)
            + (timeptr->tm_yday * 86400) + ((timeptr->tm_year-70) * 31536000)
            + (((timeptr->tm_year-69)/4) * 86400) - (((timeptr->tm_yday-1)/100) * 86400)
            + (((timeptr->tm_year+299)/400) * 86400);
    
    gmtime_r(&timer, timeptr);
    return timer;
}

struct tm *gmtime_r(const time_t *timer, struct tm *result) {
    result->tm_sec = *timer % 60;
    result->tm_min = (*timer / 60) % 60;
    result->tm_hour = (*timer / 3600) % 24;
    result->tm_yday = (*timer / 86400);     // total number of days, will fix this after calculating
    result->tm_year = (int) ((double) result->tm_yday / 365.25); // years since 1970, will fix this too
    result->tm_wday = (result->tm_yday + 4) % 7;

    // count how many days have passed before the current year in a way
    // that accounts for leap years as well
    int pastDays = 0;
    for(int y = 1970; y < result->tm_year+1970; y++) {
        if(!(y % 400) || (!(y % 4) && (y % 100))) pastDays += 366;
        else pastDays += 365;
    }

    result->tm_yday -= pastDays;    // adjust days into year
    result->tm_year += 70;          // adjust years to count from 1900

    // now we can use the day of year to calculate month and day of month
    int y = result->tm_year+1970;
    int leap = (!(y % 400)) || (!(y % 4) && (y % 100));

    result->tm_mon = 0;
    result->tm_mday = result->tm_yday;
    while(result->tm_mon < 12) {
        int daysInMonth;
        if(result->tm_mon == 1) {
            if(leap) daysInMonth = 29;
            else daysInMonth = 28;
        } else {
            daysInMonth = __daysPerMonth[result->tm_mon];
        }

        if(result->tm_mday > daysInMonth) {
            result->tm_mday -= daysInMonth;
        } else {
            break;
        }

        result->tm_mon++;
    }

    result->tm_mday++;
    return result;
}

static struct tm __gmtime;
struct tm *gmtime(const time_t *timer) {
    return gmtime_r(timer, &__gmtime);
}

char *asctime_r(const struct tm *tm, char *buf) {
    char *weekdays[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                      "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    
    sprintf(buf, "%s %s %2d %02d:%02d:%02d %d\n",
        weekdays[tm->tm_wday%7], months[tm->tm_mon%12], tm->tm_mday%32,
        tm->tm_hour%24, tm->tm_min%60, tm->tm_sec%60, tm->tm_year+1900);
    return buf;
}

static char __asctime[26];
char *asctime(const struct tm *tm) {
    return asctime_r(tm, __asctime);
}
