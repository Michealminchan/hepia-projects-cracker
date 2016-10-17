/**
 * Main program.
 * Make test on how an algorithm perform during certain time.
 * David Gonzalez
 * 15.10.2016
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

#include "password_cracker.h"
#include "password.h"
#include "time_measure.h"
#include "file.h"

#define MAIN_ARGS_COUNT 3
#define MAIN_ARGS_TIME_COUNT 4
#define MAIN_PASSWORD_TEST "password"
#define MAIN_PASSWORD_SALT_TEST "43"
#define MAIN_TIME_SEC 10

#define MAIN_PWD_MIN 0
#define MAIN_PWD_MAX 8
#define MAIN_PWD_CHARSET "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~!*"
#define MAIN_DICT_FILE "../etc/dict_stats.txt"
#define MAIN_PWD_AGAINST "431pugYzZc5QM"

/**
 * Crypt action, to measure in time.
 * @param data None.
 * @param time_param Data for perf measurement.
 */
void crypt_test(void * data, time_measure_th_t * time_param)
{
    (void)data;
    
    password_t pwd_data;
    password_init(&pwd_data);
    pwd_data.password = MAIN_PASSWORD_TEST;
    pwd_data.salt = MAIN_PASSWORD_SALT_TEST;

    time_param->element_done = 0;
    time_measure_start(&time_param->measure);

    while (time_param->measure.time_total < time_param->max_time_measure_sec)
    {
        password_crypt(&pwd_data);

        time_param->element_done++;
        time_measure_stop(&time_param->measure);
    }
}

/**
 * Bruteforce action, to measure in time.
 * @param data Cracker params.
 * @param time_param Data for perf measurement.
 */
void crack_bruteforce_test(void * data, time_measure_th_t * time_param)
{
    password_cracker_param_t * crack_params = (password_cracker_param_t*)data;

    crack_pwd_bruteforce(crack_params, time_param->id, time_param);
}
/**
 * Dict action, to measure in time.
 * @param data Cracker params.
 * @param time_param Data for perf measurement.
 */
void crack_dict_test(void * data, time_measure_th_t * time_param)
{
    password_cracker_param_t * crack_params = (password_cracker_param_t*)data;

    crack_pwd_dictionary(crack_params, time_param->id, time_param);
}

/**
 * Main function that treats input and test performance.
 * @return Program status.
 */
int main(int argc, char * argv[])
{
    int exit_status = EXIT_SUCCESS;

    if (argc >= MAIN_ARGS_COUNT)
    {
        // Args parsing
        const char * algo = argv[1];
        size_t nb_threads = strtoimax(argv[2], NULL, 10);
        size_t time_test = (argc >= MAIN_ARGS_TIME_COUNT) ? strtoimax(argv[3], NULL, 10) : MAIN_TIME_SEC;
        time_action_func func_test = NULL;

        void * params[nb_threads];
        memset(params, 0, nb_threads * sizeof(void*));
        bool has_param = false;
        file_content * dict_content = NULL;

        if (strcmp(algo, "crypt") == 0)
        {
            func_test = crypt_test;
        }
        else if (strcmp(algo, "bruteforce") == 0)
        {
            func_test = crack_bruteforce_test;

            password_cracker_param_t crack_params = {
                MAIN_PASSWORD_TEST,
                MAIN_PASSWORD_SALT_TEST,
                nb_threads,
                MAIN_PWD_MIN,
                MAIN_PWD_MAX,
                MAIN_PWD_CHARSET,
                NULL,
                0,
                MAIN_PWD_AGAINST,
                false
            };

            for (size_t i = 0; i < nb_threads; ++i)
            {
                params[i] = &crack_params;
            }
            has_param = true;
        }
        else if (strcmp(algo, "dict") == 0)
        {
            func_test = crack_dict_test;

            dict_content = file_read_full(MAIN_DICT_FILE);
            password_cracker_param_t crack_params = {
                MAIN_PASSWORD_TEST,
                MAIN_PASSWORD_SALT_TEST,
                nb_threads,
                MAIN_PWD_MIN,
                MAIN_PWD_MAX,
                MAIN_PWD_CHARSET,
                (const char**)dict_content->lines,
                dict_content->array_size,
                MAIN_PWD_AGAINST,
                false
            };

            for (size_t i = 0; i < nb_threads; ++i)
            {
                params[i] = &crack_params;
            }
            has_param = true;
        }
        else
        {
            fprintf(stderr, "Invalid algorithm: '%s'\n", algo);
        }

        if (func_test != NULL)
        {
            time_measure_action(nb_threads, time_test, func_test, (has_param == true) ? params : NULL);
        }

        file_content_free(dict_content);
    }
    else
    {
        printf("Usage: %s algo number_threads [time_to_test]\n", argv[0]);
        printf("algo can be: crypt bruteforce dict");
        exit_status = EXIT_FAILURE;
    }

    return exit_status;
}
