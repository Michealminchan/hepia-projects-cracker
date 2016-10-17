/**
 * Time measure module.
 * Some functions that help to measure execution time of a particular code.
 * David Gonzalez
 * 12.10.2016
 */

#ifndef TIME_MEASURE_H
#define TIME_MEASURE_H

#include <stdbool.h>
#include <time.h>

// Predeclaration because of circular dep
struct time_measure_th_t;
/**
 * Action callback prototype
 */
typedef void (*time_action_func)(void * data, struct time_measure_th_t * time_param);

/**
 * Structure to store measure data
 */
typedef struct time_measure_t
{
    struct timespec start;
    struct timespec finish;
    double time_total;
} time_measure_t;

/**
 * Structure for threads for measuring an action time.
 */
typedef struct time_measure_th_t
{
    size_t id;                      // Threads ID.
    time_measure_t measure;         // Data of measure.

    size_t max_time_measure_sec;    // Maximum time to test in second. Set to 0 to disable maximum.
    size_t element_done;            // Number of element done during measure.

    time_action_func action;        // Function to measure.
    void * action_param;            // Optional parameter for action.
} time_measure_th_t;

/**
 * Start a time measure.
 * @param tm Allocated structure for measure.
 */
void time_measure_start(time_measure_t * tm);

/**
 * Stop a time measure.
 * @param tm Allocated structure for measure which is started.
 */
void time_measure_stop(time_measure_t * tm);

/**
 * Start a time measure.
 * @param tm Allocated structure for measure that have started/stopped.
 * @return Number of second elasped in floating-point number.
 */
double time_measure_get_diff_sec(const time_measure_t * tm);

/**
 * Mesaure the time taken by an action and how many element can be done in this time.
 * @param nb_threads How many to invoke. 0 threads = 1 threads.
 * @param max_time Maximum time to test the action.
 * @param action Function to test.
 * @param action_params Param of each thread. Must be allocated with "nb_threads" element or NULL.
 */
void time_measure_action(size_t nb_threads, size_t max_time, time_action_func action, void ** action_params);

/**
 * Threads for measuring an action time.
 * @param data Threads data for measure.
 * @return None.
 */
void * time_measure_th(void * data);

/**
 * In an algorithm, update the time parameter. Must be done for each element done.
 * @param time_param Measure data of the algorithm.
 */
void time_measure_update_th(time_measure_th_t * time_param);

/**
 * Determine if the given time measure has timed out.
 * @param time_param Measure data of the algorithm.
 * @return TRUE if timeout, false if not.
 */
bool time_measure_has_timeout_th(time_measure_th_t * time_param);

#endif
