/**
 * Main program.
 * Claudio Sousa, David Gonzalez
 * 22.10.2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cracker.h"
#include "time_measure.h"

/**
 * Main function that cracks a pwd of 8 chars max and measure time taken.
 * @param argc arg count.
 * @param argc arg array.
 * @return The exit code
 */
int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Program expects 3 arguments:  HASH, SALT and NB_THREADS\n");
        return EXIT_FAILURE;
    }

    char *hash = argv[1];
    char *salt = argv[2];
    int threads = atoi(argv[3]);

    if (threads <= 0) {
        printf("The NB_THREADS must be and positif integer greater than 0\n");
        return EXIT_FAILURE;
    }

    struct timespec start;
    start_time_measure(&start);

    printf("Starting cracking process using %d threads\n", threads);

    char *pwd_result = crack(hash, salt, threads);

    printf("Cracking process finished after %f seconds\n", stop_time_measure(&start));
    printf("%s\n", pwd_result ? pwd_result : "No match");
    free(pwd_result);

    return EXIT_SUCCESS;
}
