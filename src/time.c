/*
 * lucerna - an implementation of the standard C library
 * Omar Elghoul, 2024
 */

#include <time.h>
#include <stddef.h>
#include <sys/time.h>

time_t time(time_t *tloc) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    if(tloc) *tloc = tv.tv_sec;
    return tv.tv_sec;
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
    int daysPerMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
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
            daysInMonth = daysPerMonth[result->tm_mon];
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