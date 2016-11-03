/**
 * Time measure module.
 * Some functions that help to measure execution time of a particular code.
 * David Gonzalez, Claudio Sousa
 * 22.10.2016
 */

#ifndef TIME_MEASURE_H
#define TIME_MEASURE_H

#include <time.h>

void start_time_measure(struct timespec *tm);
double stop_time_measure(struct timespec *tm);

#endif
