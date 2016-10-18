/**
 * Time measure module.
 * Some functions that help to measure execution time of a particular code.
 * Claudio Sousa, David Gonzalez
 * 18.10.2016
 */

#ifndef PWDGENERATOR_H
#define PWDGENERATOR_H

#include <stdbool.h>

struct pwd_generator_t;
typedef struct pwd_generator_t pwd_generator_t;

pwd_generator_t *generate_pwd_init();
char *generate_pwd_next(pwd_generator_t *generator, int jump);
void generate_pwd_free(pwd_generator_t *generator);

#endif
