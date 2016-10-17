/**
 * Threads module.
 * Helper functions to launch threads
 * David Gonzalez
 * 15.10.2016
 */

/**
 * Thread callback prototype
 */
typedef void * (*thread_func)(void *);

/**
 * Launch threads all with the same function and wait for them to finish.
 * @param nb_threads How many to invoke. 0 threads = 1 threads.
 * @param callback Function for the threads.
 * @param params Array of params for the treads. Must be allocated with "nb_threads" element or NULL.
 * @param returns Array of storage for the treads return data. Must be allocated with "nb_threads" element or NULL.
 */
void threads_launch_and_wait(size_t nb_threads, thread_func callback, void ** params, void ** returns);
