/**
 * Password cracker module.
 * With the help of the password module, efficiently crack password.
 * David Gonzalez
 * 15.10.2016
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "password_cracker.h"

#include "password.h"
#include "time_measure.h"
#include "threads.h"

/**
 * Main function of cracking password. Use multi-threads.
 * @param params Parameters of the cracker.
 * @return TRUE if success, FALSE if failed.
 */
bool crack_pwd(password_cracker_param_t * params)
{
    bool ok = true;

    memset(params->cracked_password, '\0', params->max_length + 1);

    // Brute-force algorithm doesn't allow to have more threads than chararacter in the charset
    if ((params->dictionary == NULL) && (params->nb_threads > strlen(params->charset)))
    {
        fprintf(stderr, "The brute-force algorithm doesn't allow to have more threads than characters.\n");
        fprintf(stderr, "Charset length: %lu.\n", strlen(params->charset));
        return false;
    }

    // Init threads data
    password_cracker_th_param_t * th_pool_data_ptr[params->nb_threads];
    password_cracker_th_param_t th_pool_data[params->nb_threads];
    for (size_t i = 0; i < params->nb_threads; ++i)
    {
        th_pool_data[i].id = i;
        th_pool_data[i].params = params;

        th_pool_data_ptr[i] = &th_pool_data[i];
    }

    // Launch
    threads_launch_and_wait(params->nb_threads, crack_pwd_th, (void **)th_pool_data_ptr, NULL);

    if (params->cracked_password[0] == '\0')
    {
        ok = false;
    }

    return ok;
}

/**
 * Thread function that crack a password.
 * @param data Data structure with the password to crack and synchro informations.
 * @return None
 */
void * crack_pwd_th(void * data)
{
    password_cracker_th_param_t * params_th = (password_cracker_th_param_t*)data;

    if (params_th->params->dictionary == NULL)
    {
        crack_pwd_bruteforce(params_th->params, params_th->id, NULL);
    }
    else
    {
        crack_pwd_dictionary(params_th->params, params_th->id, NULL);
    }

    return NULL;
}

/**
 * Brute-force a crypted password.
 * @param params Parameters of the cracker.
 * @param offset Start offset in case of multithreading.
 * @param time_param Data for perf measurement. Can be NULL.
 */
void crack_pwd_bruteforce(password_cracker_param_t * params, size_t offset, time_measure_th_t * time_param)
{
    // Init receving password and character index to store progress
    char password_test[params->max_length + 1];
    memset(password_test, '\0', params->max_length + 1);
    size_t password_test_idx[params->max_length + 1];
    memset(password_test_idx, 0, (params->max_length + 1) * sizeof(size_t));
    
    password_t pwd_data;
    password_init(&pwd_data);
    pwd_data.password = password_test;
    pwd_data.salt = params->salt;

    // Convenient (shortcut)
    size_t charset_length = strlen(params->charset) + 1;
    unsigned int step = (params->nb_threads != 0) ? params->nb_threads : 1; // Avoid step of 0

    // Loop variables
    size_t i = 0;
    size_t p = 0;

    // Min length: set all char in range to the start one
    for (size_t i = 0; i < params->min_length; ++i)
    {
        password_test[i] = params->charset[0];
        password_test_idx[i] = 1;
    }

    // Until the last character isn't the last char tested.
    while ((password_test_idx[params->max_length - 1] < charset_length) &&
            (params->done_flag == false) && !time_measure_has_timeout_th(time_param)
    ) {
        // Do the first position loop
        // Start at the first char, finish at end char
        // Step is the number of threads
        for (i = offset; (i < charset_length) && (params->done_flag == false); i += step)
        {
            password_test[0] = params->charset[i];

            if (password_validate(&pwd_data, params->crypted_password) == true)
            {
                params->done_flag = true;
                strncpy(params->cracked_password, password_test, params->max_length + 1);
            }

            // Stats
            time_measure_update_th(time_param);
        }
        // Get back "i" for increment
        password_test_idx[0] = i;

        // Increment next char positions (0 to params->max_length), like minutes-hours:
        // when arriving to max, simply set the position to the start and increment the next.
        for (p = 0;
            (p < (params->max_length - 1)) && (params->done_flag == false) && (password_test_idx[p] >= charset_length);
            ++p
        ) {
            password_test_idx[p] = 1;
            password_test[p] = params->charset[password_test_idx[p] - 1];

            password_test_idx[p + 1]++;
            password_test[p + 1] = params->charset[password_test_idx[p + 1] - 1];
        }
    }
}

/**
 * Use a loaded dictionary to crack a crypted password.
 * @param params Parameters of the cracker.
 * @param offset Start offset in case of multithreading.
 * @param time_param Data for perf measurement. Can be NULL.
 */
void crack_pwd_dictionary(password_cracker_param_t * params, size_t offset, time_measure_th_t * time_param)
{
    unsigned int step = (params->nb_threads != 0) ? params->nb_threads : 1; // Avoid step of 0
    
    password_t pwd_data;
    password_init(&pwd_data);
    pwd_data.password = NULL;
    pwd_data.salt = params->salt;

    // Test each word, starting from offset and going by step (which is the number of threads)
    for (size_t i = offset;
        (i < params->dictionary_size) && (params->done_flag == false) && !time_measure_has_timeout_th(time_param);
        i+=step
    ) {
        // Test password match
        pwd_data.password = params->dictionary[i];
        if (password_validate(&pwd_data, params->crypted_password) == true)
        {
            params->done_flag = true;
            strncpy(params->cracked_password, params->dictionary[i], params->max_length + 1);
        }

        // Stats
        time_measure_update_th(time_param);
    }
}
