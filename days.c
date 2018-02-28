#include "days.h"
#include "debug.h"
#include <assert.h>


int lookup_days_in_month(struct tm* t)
{
    const int DAYS_IN_MONTH[12] = {
        31, //Jan
        28, //Feb
        31, //Mar
        30, //Apr
        31, //May
        30, //Jun
        31, //Jul
        31, //Aug
        30, //Sept
        31, //Oct
        30, //Nov
        31 //Dec
    };
    int month = t->tm_mon + 1;
    int year = t->tm_year + 1900;

    int total_days;
    total_days = DAYS_IN_MONTH[month - 1];

    if (month == 2) {
        if (is_leap_year(year) == 1) {
            total_days++;
        }
    }

    return total_days;
}

int is_leap_year(int year)
{
    if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0)) {
        return 1;
    }
    return 0;
}

int is_weekday(int d, int m, int y)
{
    int day = (d += m < 3 ? y-- : y - 2, 23 * m / 9 + d + 4 + y / 4 - y / 100 + y / 400) % 7;

    //0 = sunday, 1 = monday, 2 = tuesday, 3 = wednesday, 4 = thursday, 5 = friday, 6 = saturday
    if (day == 0 || day == 6) {
        return 0;
    }
    return 1;
}

int is_weekend(int d, int m, int y)
{
    int weekday = is_weekday(d, m, y);

    if (weekday == 1) {
        return 0;
    }
    return 1;
}

int count_weekdays(int from_day, int to_day, int month, int year)
{
    int weekdays = 0;
    int d = 0;

    if ((from_day < 0) && (to_day > 31)) {
        return -1;
    }

    if ((from_day == 1) && (to_day > 29)) {
        weekdays = 20;
        d = 29;
    } else {
        d = from_day;
    }

    //work out the remaining work days of the month
    for (; d <= to_day; ++d) {
        int weekday = is_weekday(d, month + 1, year + 1900);
        if (weekday == 1) {
            weekdays++;
        }
    }
    return weekdays;
}

int number_of_days(struct tm* from, struct tm* to, int working_days_only)
{
    if (from->tm_year + 1900 < 1900) {
        return -1;
    }

    //accumulator of number of days
    int days = 0;

    //tracks the year we are working with
    int year_loop = from->tm_year;

    //tracks the current month we are counting
    int month_loop = 0; //from->tm_mon;

    //sets the max month we will include
    int max_month = 11; //jan = 0

    for (year_loop = from->tm_year; year_loop <= to->tm_year; year_loop++) {

        //same year
        if ((year_loop == from->tm_year) && (year_loop == to->tm_year)) {
            max_month = to->tm_mon;
            month_loop = from->tm_mon;
        } else if ((year_loop == from->tm_year) && (year_loop < to->tm_year)) {
            max_month = 11;
            month_loop = from->tm_mon;

            //a year that is not the start or end years
        } else if ((year_loop > from->tm_year) && (year_loop < to->tm_year)) {
            max_month = 11;
            month_loop = 0;

            //the end year
        } else if (year_loop == to->tm_year) {
            max_month = to->tm_mon;
            month_loop = 0;
        }

        for (; month_loop <= max_month; month_loop++) {

            //we are in the start month, so we more than likly be counting from day 1
            if ((year_loop == from->tm_year) && (month_loop == from->tm_mon)) {

                if (working_days_only == 1) {
                    int tmp_days = lookup_days_in_month(from);
                    int weekdays = count_weekdays(from->tm_mday, tmp_days, month_loop, year_loop);

                    days += weekdays;
                } else {
                    days += (lookup_days_in_month(from) - from->tm_mday);
                }

                //we are in the end month of the end year of the time span
            } else if (year_loop == to->tm_year && month_loop == to->tm_mon) {
                int im = lookup_days_in_month(to);

                if (working_days_only == 1) {
                    int weekdays = count_weekdays(1, to->tm_mday, month_loop, year_loop);
                    days += weekdays;
                } else {
                    int delta = im - to->tm_mday;
                    days += (im - delta);
                }

                //every full month inbetween the start and the end dates
            } else {
                struct tm t = (struct tm){ 0 };
                t.tm_year = year_loop;
                t.tm_mon = month_loop;
                if (working_days_only == 1) {
                    int weekdays = count_weekdays(1, lookup_days_in_month(&t), month_loop, year_loop);
                    days += weekdays;
                } else {
                    days += (lookup_days_in_month(&t));
                }
            }
        }
    }

    return days;
}

//returns number of seconds between time_t and time_t
int calc_duration(time_t to, time_t from, ydhms_t* dur)
{
    int diff = (int)difftime(to, from);

    int res = seconds_to_ydhms(diff, dur);
    return res;
}

int working_seconds_to_ydmhs(int dur_in_seconds, int workday_duration_in_seconds, ydhms_t* dur)
{
    assert(workday_duration_in_seconds >= 0);
    int days = dur_in_seconds / workday_duration_in_seconds;

    int remaining_seconds = dur_in_seconds % workday_duration_in_seconds;

    seconds_to_ydhms(remaining_seconds, dur);

    dur->days = days;
    return 1;
}

int seconds_to_ydhms(int dur_in_seconds, ydhms_t* dur)
{
    //minute
    int tmp_m = dur_in_seconds / 60;

    //seconds
    int seconds = dur_in_seconds % 60;

    //hours
    int tmp_h = tmp_m / 60;

    //actual minutes
    int mins = tmp_m % 60;

    int ddays = tmp_h / 24;
    int hours = tmp_h % 24;

    // dur->years = 0;
    dur->days = ddays;
    dur->hours = hours;
    dur->minutes = mins;
    dur->seconds = seconds;

    return 1;
}

int dt_range_in_seconds(dt_range_t d)
{
    return (int)difftime(d.to, d.from);
}

// calcualte_working_time_span
int calculate_time_span(time_t current_time_raw, struct tm end_date, dt_range_t working_hours, int* working_time_span)
{
    struct tm current_local;
    current_local = *localtime(&current_time_raw);

    int working_hours_in_seconds = (int)difftime(working_hours.to, working_hours.from);

    int days = pending_days(current_local, end_date, EXCLUDE_START_DAY | EXCLUDE_END_DAY);

    int seconds = days * working_hours_in_seconds;

    // we are in the curret working hours range
    int remaining_seconds = 0;

    //if true, this is the last day
    if ((current_local.tm_year == end_date.tm_year) && (current_local.tm_mday == end_date.tm_mday) && (current_local.tm_mon == end_date.tm_mon)) {
        if ((current_time_raw >= working_hours.from) && (current_time_raw <= working_hours.to)) {
            remaining_seconds = (int)difftime(working_hours.to, current_time_raw);
        }
    } else {
        int d = current_local.tm_mday;
        int m = current_local.tm_mon + 1;
        int y = current_local.tm_year + 1900;

        if ((is_weekday(d, m, y) == 1) && (current_time_raw > working_hours.from) && (current_time_raw < working_hours.to)) {
            remaining_seconds = (int)difftime(working_hours.to, current_time_raw);
            assert(remaining_seconds > 0);
        } else {
            // add the full working hours to cater for the next working day, which is Monday
            remaining_seconds = working_hours_in_seconds;
        }

        //adds the full working day for the last day
        int t = (int)difftime(working_hours.to, working_hours.from);
        remaining_seconds += t;
    }

    int r = seconds + remaining_seconds;

    *working_time_span = r;
    return 1;
}

int pending_days(struct tm from, struct tm to, int flags)
{
    int strip_days = 0;

    if (flags & EXCLUDE_START_DAY) {
        strip_days++;
    }
    if (flags & EXCLUDE_END_DAY) {
        strip_days++;
    }

    int days = number_of_days(&from, &to, 1);
    int res = days - strip_days;

    return res;
}
