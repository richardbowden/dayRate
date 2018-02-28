#include "days.h"
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CONTRACT_END_DATE "16/03/2018"
#define CONTRACT_END_TIME "16:30:00"
#define CONTRACT_END CONTRACT_END_DATE " " CONTRACT_END_TIME

#define DAY_START_TIME "09:00:00"
#define DAY_END_TIME "17:00:00"

int main(int argc, char* argv[])
{

    //get end date
    time_t current_time;
    time(&current_time);

    struct tm* current_time_local;
    current_time_local = localtime(&current_time);

    struct tm* contract_end_date = (struct tm*)calloc(1, sizeof(*contract_end_date));

    strptime(CONTRACT_END, "%d/%m/%Y %H:%M:%S", contract_end_date);

    time_t contract_end = mktime(contract_end_date);

    //working hours
    //copy current_time_local so that we can do proper time math but also keep the correct date for weekend checking
    struct tm day_start = *current_time_local;
    struct tm day_finish = *current_time_local;

    strptime(DAY_START_TIME, "%H:%M:%S", &day_start);
    strptime(DAY_END_TIME, "%H:%M:%S", &day_finish);

    time_t day_start_raw = mktime(&day_start);
    time_t day_finish_raw = mktime(&day_finish);

    dt_range_t working_hours = { day_start_raw, day_finish_raw };
    int total_working_day_in_seconds = dt_range_in_seconds(working_hours);

    int* working_time_in_seconds = malloc(sizeof *working_time_in_seconds);

    ydhms_t* dur = (ydhms_t*)calloc(1, sizeof(dur));
    ydhms_t* working_dur = (ydhms_t*)calloc(1, sizeof(working_dur));
    while (1) {
        time(&current_time);

        calculate_time_span(current_time, *contract_end_date, working_hours, working_time_in_seconds);
        calc_duration(contract_end, current_time, dur);
        working_seconds_to_ydmhs(*working_time_in_seconds, total_working_day_in_seconds, working_dur);

        
        printf("\r%d days %d hours %d minutes and %d seconds --- %d Working days, %d hours %d minutes and seconds %d...", dur->days, dur->hours, dur->minutes, dur->seconds, working_dur->days, working_dur->hours, working_dur->minutes, working_dur->seconds);
    sleep(1);
        fflush(stdout);
    }
}
