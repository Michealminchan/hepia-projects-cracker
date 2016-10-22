/**
 * Time measure module.
 * Some functions that help to measure execution time of a particular code.
 * David Gonzalez, Claudio Sousa
 * 22.10.2016
 */

#include <stdio.h>
#include "time_measure.h"

static const double NANO_SECONDS_IN_SECOND = 1000000000;

/**
 * Starts a time measure.
 * @param tm Structure to hold the starting time.
 */
void start_time_measure(struct timespec *tm) {
    if (clock_gettime(CLOCK_MONOTONIC, tm) != 0)
        perror("Time measure start failed");
}

/**
 * Stops a time measure and return time diff
 * @param start_time Measure starting time.
 * @return The time delta in seconds
 */
double stop_time_measure(struct timespec *start_time) {
    double elapsed = 0;
    struct timespec end_time;

    if (clock_gettime(CLOCK_MONOTONIC, &end_time) == 0) {
        elapsed = end_time.tv_sec - start_time->tv_sec;
        elapsed += (end_time.tv_nsec - start_time->tv_nsec) / NANO_SECONDS_IN_SECOND;
    } else
        perror("Time measure stop failed");

    return elapsed;
}
