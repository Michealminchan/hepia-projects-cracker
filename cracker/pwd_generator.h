/**
 * Password generator module.
 * Generate password by step using indexes from the charset (VALID_SYMBOLS)
 * Claudio Sousa, David Gonzalez
 * 22.10.2016
 */

#ifndef PWDGENERATOR_H
#define PWDGENERATOR_H

struct pwd_generator_t;
typedef struct pwd_generator_t pwd_generator_t;

pwd_generator_t *generate_pwd_init();
char *generate_pwd_next(pwd_generator_t *generator, int jump);
void generate_pwd_free(pwd_generator_t *generator);

#endif
