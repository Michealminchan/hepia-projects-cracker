/**
 * Threads module.
 * Helper functions to launch threads
 * David Gonzalez
 * 15.10.2016
 */

#include <stdio.h>
#include <pthread.h>

#include "threads.h"

/**
 * Launch threads all with the same function and wait for them to finish.
 * @param nb_threads How many to invoke. 0 threads = 1 threads.
 * @param callback Function for the threads.
 * @param params Array of params for the treads. Must be allocated with "nb_threads" element or NULL.
 * @param returns Array of storage for the treads return data. Must be allocated with "nb_threads" element or NULL.
 */
void threads_launch_and_wait(size_t nb_threads, thread_func callback, void ** params, void ** returns)
{
    nb_threads = (nb_threads > 0) ? nb_threads : 1;

    pthread_t th_pool[nb_threads];

    // Init data and launch threads
    for (size_t i = 0; i < nb_threads; ++i)
    {
        void * param_element = (params != NULL) ? params[i] : NULL;

        if (pthread_create(&th_pool[i], NULL, callback, param_element) != 0)
        {
            perror("Thread creation failed");
        }
    }

    // Wait threads end (they auto stop themself with the done_flag attribute)
    for (size_t i = 0; i < nb_threads; ++i)
    {
        void ** return_element = (returns != NULL) ? &returns[i] : NULL;

        if (pthread_join(th_pool[i], return_element) != 0)
        {
            perror("Thread join failed");
        }
    }
}
