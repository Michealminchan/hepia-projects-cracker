/**
 * Time measure module.
 * Some functions that help to measure execution time of a particular code.
 * David Gonzalez, Claudio Sousa
 * 18.10.2016
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
 * @param startTime Measure starting time.
 * @return The time delta in seconds
 */
double stop_time_measure(struct timespec *startTime) {
    double elapsed = 0;
    struct timespec endTime;
    if (clock_gettime(CLOCK_MONOTONIC, &endTime) == 0) {
	elapsed = endTime.tv_sec - startTime->tv_sec;
	elapsed += (endTime.tv_nsec - startTime->tv_nsec) / NANO_SECONDS_IN_SECOND;
    } else
	perror("Time measure stop failed");
    return elapsed;
}
