require cmake and all that crap

`cmake .`
make


to set working hours and start / end date check out the DEFINE's in main.c

main.c

```
CONTRACT_END_DATE
CONTRACT_END_TIME
DAY_START_TIME
DAY_END_TIME
```


noddy test

```gcc -DDEBUG tests_days.c days.c -o tests
./tests
```
