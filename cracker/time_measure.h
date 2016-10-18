/**
 * Time measure module.
 * Some functions that help to measure execution time of a particular code.
 * David Gonzalez, Claudio Sousa
 * 18.10.2016
 */

#ifndef TIME_MEASURE_H
#define TIME_MEASURE_H

#include <stdbool.h>
#include <time.h>


void start_time_measure(struct timespec *tm);
double stop_time_measure(struct timespec *tm);

#endif
