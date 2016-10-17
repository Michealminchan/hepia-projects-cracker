#define _GNU_SOURCE
#include <crypt.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pwdGenerator.h"
#include "cracker.h"

typedef struct {
  int start;
  int step;
  char *hash;
  char *salt;
  bool *found;
  char *pwd;
} crackWorker_t;

static void *crackWorker(void *params) {
  crackWorker_t *workerParams = (crackWorker_t *)params;

  struct crypt_data cdata;
  cdata.initialized = 0;

  pwdGenerator_t pwdGenerator;
  pwdGenerator.initialized = false;

  char *testPwd = getNthPassword(workerParams->start, &pwdGenerator);
  do {
    char *hash = crypt_r(testPwd, workerParams->salt, &cdata);

    if (!strcmp(hash, workerParams->hash)) {
      *(workerParams->found) = true;
      workerParams->pwd = testPwd;
      printf("Found %s\n", testPwd);
      return NULL;
    }
  } while ((testPwd = getNthPassword(workerParams->step, &pwdGenerator)) && !*(workerParams->found));

  return NULL;
}
// crack_t
crack_t *crack(char *hash, char *salt, int threadCount) {
  pthread_t threads[threadCount];
  crack_t *res = (crack_t *)malloc(sizeof(crack_t));
  res->found = false;
  crackWorker_t workerParams[threadCount];
  for (int i = 0; i < threadCount; i++) {
    workerParams[i].start = i;
    workerParams[i].step = threadCount;
    workerParams[i].hash = hash;
    workerParams[i].salt = salt;
    workerParams[i].found = &(res->found);
    workerParams[i].pwd = res->pwd;
    if (pthread_create(&threads[i], NULL, crackWorker, &(workerParams[i]))) {
      fprintf(stderr, "pthread_create failed!\n");
      return res;
    }
  }
  for (int i = 0; i < threadCount; i++) {
    if (pthread_join(threads[i], NULL)) {
      fprintf(stderr, "pthread_join failed!\n");
    }
  }

  return res;
}