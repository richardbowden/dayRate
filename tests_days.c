#include "days.h"
#include "debug.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define START_DATE "15/03/1980 09:00"
#define END_DATE "03/06/1980 16:30"

#define START_DATE_MULTI_YEAR "15/03/1980 09:00"
#define END_DATE_MULTI_YEAR "03/06/1983 16:30"

#define START_DATE_MULTI_YEAR_LONGER "6/04/2004 09:00"
#define END_DATE_MULTI_YEAR_LONGER "11/02/2018 16:30"

int main()
{
    struct tm start = { 0 };
    struct tm end = { 0 };

    strptime(START_DATE, "%d/%m/%Y %k:%M", &start);
    strptime(END_DATE, "%d/%m/%Y %k:%M", &end);

    int days = number_of_days(&start, &end,0);
    assert(days == 80);

    struct tm start1 = { 0 };
    struct tm end1 = { 0 };
    strptime(START_DATE_MULTI_YEAR, "%d/%m/%Y %k:%M", &start1);
    strptime(END_DATE_MULTI_YEAR, "%d/%m/%Y %k:%M", &end1);

    int days1 = number_of_days(&start1, &end1, 0);
    assert(days1 == 1175);

    struct tm start2 = { 0 };
    struct tm end2 = { 0 };
    strptime(START_DATE_MULTI_YEAR_LONGER, "%d/%m/%Y %k:%M", &start2);
    strptime(END_DATE_MULTI_YEAR_LONGER, "%d/%m/%Y %k:%M", &end2);

    int days2 = number_of_days(&start2, &end2, 0);
    assert(days2 == 5059);

    DEBUG_PRINT("meh from testing file, if notthing else is showen, tests passed\n");

    return 1;
}