/**
 * Password cracker module.
 * Bruteforce a password using multithreading.
 * Claudio Sousa, David Gonzalez
 * 22.10.2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <crypt.h>
#include <pthread.h>
#include "cracker.h"
#include "pwd_generator.h"

typedef struct {
    int start;
    int step;
    char *hash;
    char *salt;
    bool *found;
    char **pwd;
} crack_worker_t;

/**
 * Threads function that will crack the given password in "params"
 * @param params Thread parameters
 */
static void *crack_worker(void *params) {
    crack_worker_t *worker_params = (crack_worker_t *)params;

    struct crypt_data cdata;
    cdata.initialized = 0;

    pwd_generator_t *pwd_generator = generate_pwd_init();

    if (pwd_generator != NULL) {
        char *test_pwd = generate_pwd_next(pwd_generator, worker_params->start);

        do {
            char *hash = crypt_r(test_pwd, worker_params->salt, &cdata);

            if (!strcmp(hash, worker_params->hash)) {
                *(worker_params->found) = true;
                *(worker_params->pwd) = (char *)malloc(sizeof(char) * (strlen(test_pwd) + 1));
                strcpy(*(worker_params->pwd), test_pwd);
            }
        } while (!*(worker_params->found) && (test_pwd = generate_pwd_next(pwd_generator, worker_params->step)));

        generate_pwd_free(pwd_generator);
    }

    return NULL;
}

/**
 * Launch the threads that will crack the given password
 * @param hash Hashed password to crack
 * @param salt Salt used to hash the given hashed password
 * @param thread_count Number of threads to use
 * @return Found password or NULL if not found
 */
char *crack(char *hash, char *salt, int thread_count) {
    pthread_t threads[thread_count];
    bool found = false;
    char *pwd_res = NULL;
    crack_worker_t worker_params[thread_count];

    for (int i = 0; i < thread_count; i++) {
        worker_params[i].start = i;
        worker_params[i].step = thread_count;
        worker_params[i].hash = hash;
        worker_params[i].salt = salt;
        worker_params[i].found = &found;
        worker_params[i].pwd = &pwd_res;

        if (pthread_create(&threads[i], NULL, crack_worker, &(worker_params[i]))) {
            fprintf(stderr, "pthread_create failed!\n");
            return NULL;
        }
    }

    for (int i = 0; i < thread_count; i++) {
        if (pthread_join(threads[i], NULL)) {
            fprintf(stderr, "pthread_join failed!\n");
        }
    }

    return pwd_res;
}
