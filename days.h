#include <time.h>

typedef struct {
    int years;
    int days;
    int hours;
    int minutes;
    int seconds;
} ydhms_t;

typedef struct dt_range {
    time_t from;
    time_t to;
} dt_range_t;

int is_leap_year(int year);
int number_of_days(struct tm* from, struct tm* to, int working_days_only);
int pending_days(struct tm from, struct tm to, int flags);
int is_weekday(int d, int m, int y);
int calc_duration(time_t to, time_t from, ydhms_t* dur);
int seconds_to_ydhms(int dur_in_seconds, ydhms_t* dur);
// int calculate_time_span(struct tm current_local, struct tm end_date, dt_range_t working_hours, int* working_time_span);
int calculate_time_span(time_t current_time_raw, struct tm end_date, dt_range_t working_hours, int* working_time_span);
int count_weekdays(int from_day, int to_day, int month, int year);
int working_seconds_to_ydmhs(int dur_in_seconds, int workday_duration_in_seconds, ydhms_t* dur);
int is_weekend(int d, int m, int y);
int dt_range_in_seconds(dt_range_t d);

typedef enum ucl_parser_flags {
    INCLUDE_ALL_DAYS_DEFAULT = 0,
    EXCLUDE_END_DAY = (1 << 0),
    EXCLUDE_START_DAY = (1 << 1),
} day_include_flags_t;
