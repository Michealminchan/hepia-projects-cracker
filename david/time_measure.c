/**
 * Time measure module.
 * Some functions that help to measure execution time of a particular code.
 * David Gonzalez
 * 12.10.2016
 */

#include <stdio.h>

#include "time_measure.h"

#include "threads.h"

/**
 * Start a time measure.
 * @param tm Allocated structure for measure.
 */
void time_measure_start(time_measure_t * tm)
{
    if (clock_gettime(CLOCK_MONOTONIC, &tm->start) != 0)
    {
        perror("Time measure start failed");
    }

    // Also init end to avoid weird time without a stop
    time_measure_stop(tm);
}

/**
 * Stop a time measure.
 * @param tm Allocated structure for measure which is started.
 */
void time_measure_stop(time_measure_t * tm)
{
    if (clock_gettime(CLOCK_MONOTONIC, &tm->finish) != 0)
    {
        perror("Time measure stop failed");
    }

    tm->time_total = time_measure_get_diff_sec(tm);
}

/**
 * Start a time measure.
 * @param tm Allocated structure for measure that have started/stopped.
 * @return Number of second elasped in floating-point number.
 */
double time_measure_get_diff_sec(const time_measure_t * tm)
{
    double elapsed = tm->finish.tv_sec - tm->start.tv_sec;
    elapsed += (tm->finish.tv_nsec - tm->start.tv_nsec) / 1000000000.0;

    return elapsed;
}

/**
 * Mesaure the time taken by an action and how many element can be done in this time.
 * @param nb_threads How many to invoke. 0 threads = 1 threads.
 * @param max_time Maximum time to test the action.
 * @param action Function to test.
 * @param action_params Param of each thread. Must be allocated with "nb_threads" element or NULL.
 */
void time_measure_action(size_t nb_threads, size_t max_time, time_action_func action, void ** action_params)
{
    // Init threads data
    time_measure_th_t * th_pool_data_ptr[nb_threads];
    time_measure_th_t th_pool_data[nb_threads];
    for (size_t i = 0; i < nb_threads; ++i)
    {
        th_pool_data[i].id = i;

        th_pool_data[i].max_time_measure_sec = max_time;
        th_pool_data[i].action = action;
        th_pool_data[i].action_param = (action_params != NULL) ? action_params[i] : NULL;
        th_pool_data[i].element_done = 0;
        time_measure_start(&th_pool_data[i].measure);

        th_pool_data_ptr[i] = &th_pool_data[i];
    }

    // Launch
    threads_launch_and_wait(nb_threads, time_measure_th, (void **)th_pool_data_ptr, NULL);

    // Show result
    double time_total = 0;
    size_t element_done_total = 0;
    for (size_t i = 0; i < nb_threads; ++i)
    {
        printf("Thread %03lu:            %lu\n", i, th_pool_data[i].element_done);

        time_total = (th_pool_data[i].measure.time_total > time_total) ? th_pool_data[i].measure.time_total : time_total;
        element_done_total += th_pool_data[i].element_done;
    }

    printf("\nTotal:                 %lu\n", element_done_total);
    printf("Elements per second:   %f\n", element_done_total / time_total);
    printf("Element time (us):     %f\n", (time_total / element_done_total) * 1000000);
}

/**
 * Threads for measuring an action time.
 * @param data Threads data for measure.
 * @return None.
 */
void * time_measure_th(void * data)
{
    time_measure_th_t * time_param = (time_measure_th_t*)data;

    time_param->action(time_param->action_param, time_param);

    return NULL;
}

/**
 * In an algorithm, update the time parameter. Must be done for each element done.
 * @param time_param Measure data of the algorithm.
 */
void time_measure_update_th(time_measure_th_t * time_param)
{
    if (time_param != NULL)
    {
        time_param->element_done++;
        time_measure_stop(&time_param->measure);
    }
}

/**
 * Determine if the given time measure has timed out.
 * @param time_param Measure data of the algorithm.
 * @return TRUE if timeout, false if not.
 */
bool time_measure_has_timeout_th(time_measure_th_t * time_param)
{
    return (time_param != NULL) && (time_param->measure.time_total > time_param->max_time_measure_sec);
}
