/**
 * Main program.
 * Crack hashed password using threads.
 * Two methods are available: brute force and dictionary.
 * David Gonzalez
 * 15.10.2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "password_cracker.h"
#include "time_measure.h"
#include "file.h"

#define MAIN_ARGS_COUNT 4
#define MAIN_ARGS_DICT_COUNT 5
#define MAIN_PWD_MIN 0
#define MAIN_PWD_MAX 8
#define MAIN_PWD_CHARSET "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~!*"

/**
 * Main function that treats input and give them to the cracker.
 * @return Program status.
 */
int main(int argc, char * argv[])
{
    int exit_status = EXIT_SUCCESS;

    printf("Welcome to this password cracker.\n");

    if (argc >= MAIN_ARGS_COUNT)
    {
        // If have a dictionary, load it
        file_content * dict_content = NULL;
        if (argc >= MAIN_ARGS_DICT_COUNT)
        {
            dict_content = file_read_full(argv[4]);
        }

        // Crack params
        char cracked_password[MAIN_PWD_MAX + 1] = "";
        password_cracker_param_t crack_params = {
            argv[1],
            argv[2],
            strtoimax(argv[3], NULL, 10),
            MAIN_PWD_MIN,
            MAIN_PWD_MAX,
            MAIN_PWD_CHARSET,
            (dict_content != NULL) ? (const char**)dict_content->lines : NULL,
            (dict_content != NULL) ? dict_content->array_size : 0,
            cracked_password,
            false
        };
        time_measure_t measure;

        time_measure_start(&measure);
        bool ok = crack_pwd(&crack_params);
        time_measure_stop(&measure);

        // Free dict
        file_content_free(dict_content);

        // Show result
        if (ok == true)
        {
            printf("Password cracked ! It is '%s'\n", cracked_password);
            printf("Time elapsed: %f\n", measure.time_total);
        }
        else
        {
            printf("No match found.\n");
            exit_status = EXIT_FAILURE;
        }
    }
    else
    {
        printf("Usage: %s crypted_pwd used_salt number_threads [dict_file_path]\n", argv[0]);
        exit_status = EXIT_FAILURE;
    }

    return exit_status;
}
