#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cracker.h"

int main(int argc, char *argv[])
{

    if (argc != 4)
    {
        printf("Program expects 3 arguments:  HASH, SALT and NB_THREADS\n");
        return EXIT_FAILURE;
    }

    char *hash = argv[1];
    char *salt = argv[2];
    int threads = atoi(argv[3]);

    if (!threads)
    {
        printf("The NB_THREADS must be and positif integer greater than 0\n");
        return EXIT_FAILURE;
    }

    struct timespec start, finish;
    clock_gettime(CLOCK_MONOTONIC, &start);

    printf("Starting cracking process using %d threads\n", threads);

    crack_t *result = crack(hash, salt, threads);

    clock_gettime(CLOCK_MONOTONIC, &finish);
    double elapsed = finish.tv_sec - start.tv_sec;
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("Cracking process finished after %f seconds\n", elapsed);
    if (!result->found)
        printf("No match");

    free(result);
    return EXIT_SUCCESS;
}
