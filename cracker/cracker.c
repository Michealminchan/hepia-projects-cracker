/**
 * Time measure module.
 * Some functions that help to measure execution time of a particular code.
 * Claudio Sousa, David Gonzalez
 * 18.10.2016
 */

#include <crypt.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pwd_generator.h"
#include "cracker.h"

typedef struct {
  int start;
  int step;
  char *hash;
  char *salt;
  bool *found;
  char **pwd;
} crackWorker_t;

static void *crackWorker(void *params) {
  crackWorker_t *workerParams = (crackWorker_t *)params;

  struct crypt_data cdata;
  cdata.initialized = 0;

  pwd_generator_t *pwd_generator = generate_pwd_init();

  char *testPwd = generate_pwd_next(pwd_generator, workerParams->start);
  do {
    char *hash = crypt_r(testPwd, workerParams->salt, &cdata);
    
    if (!strcmp(hash, workerParams->hash)) {
      *(workerParams->found) = true;
      *(workerParams->pwd) = (char *)malloc(sizeof(char) * (strlen(testPwd) + 1));
      strcpy(*(workerParams->pwd), testPwd);
    }
  } while (!*(workerParams->found) && (testPwd = generate_pwd_next(pwd_generator, workerParams->step)));

  generate_pwd_free(pwd_generator);

  return NULL;
}

char *crack(char *hash, char *salt, int threadCount) {
  pthread_t threads[threadCount];
  bool found = false;
  char *pwd_res = NULL;
  crackWorker_t workerParams[threadCount];
  for (int i = 0; i < threadCount; i++) {
    workerParams[i].start = i;
    workerParams[i].step = threadCount;
    workerParams[i].hash = hash;
    workerParams[i].salt = salt;
    workerParams[i].found = &found;
    workerParams[i].pwd = &pwd_res;

    if (pthread_create(&threads[i], NULL, crackWorker, &(workerParams[i]))) {
      fprintf(stderr, "pthread_create failed!\n");
      return NULL;
    }
  }

  for (int i = 0; i < threadCount; i++) {
    if (pthread_join(threads[i], NULL)) {
      fprintf(stderr, "pthread_join failed!\n");
    }
  }

  return pwd_res;
}