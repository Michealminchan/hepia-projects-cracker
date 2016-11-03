/**
 * Password generator module.
 * Generate password by step using indexes from the charset (VALID_SYMBOLS)
 * Claudio Sousa, David Gonzalez
 * 22.10.2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pwd_generator.h"

static const int MAX_PWD_SIZE = 8;

static const char VALID_SYMBOLS[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~!*";
static const int SYMBOL_COUNT = sizeof(VALID_SYMBOLS) / sizeof(VALID_SYMBOLS[0]) - 1;

struct pwd_generator_t {
    char *pwd;
    char *pwd_chars; // VALID_SYMBOLS index of the chars in pwd
};

/**
 * Recursively Increment the password generator char at 'pos' by 'how_many'.
 * If overflows, increment the next position.
 * @param generator The working generator
 * @param pos The position of the char to modify
 * @param how_many The step to increase the char
 * @return The new password
 */
static char *incr_pwd_char(pwd_generator_t *generator, int pos, int how_many) {
    int sum = (int)generator->pwd_chars[pos] + how_many;
    int new_char = sum % SYMBOL_COUNT;
    int sum_overflow = sum / SYMBOL_COUNT;

    generator->pwd_chars[pos] = new_char;
    generator->pwd[pos] = VALID_SYMBOLS[new_char];

    if (sum_overflow) {
        if (pos == MAX_PWD_SIZE - 1)
            return NULL;
        return incr_pwd_char(generator, pos + 1, sum_overflow);
    }

    return generator->pwd;
}

/**
 * Create and initializes a pwd_generator_t
 * @return The new pwd_generator_t
 */
pwd_generator_t *generate_pwd_init() {
    pwd_generator_t *generator = (pwd_generator_t *)malloc(sizeof(pwd_generator_t));

    if (generator != NULL) {
        generator->pwd = (char *)malloc(sizeof(char) * (MAX_PWD_SIZE + 1));
        generator->pwd_chars = (char *)malloc(sizeof(char) * MAX_PWD_SIZE);

        if (generator->pwd != NULL)
            memset(generator->pwd, 0, MAX_PWD_SIZE + 1);
        if (generator->pwd_chars != NULL)
            memset(generator->pwd_chars, -1, MAX_PWD_SIZE);
    }
    else
        perror("Generator initialisation failed");

    return generator;
}

/**
 * Moves the password generator to the next password
 * @param generator The working pwd generator
 * @param jump The nth pwd to jump to
 * @return The new password
 */
char *generate_pwd_next(pwd_generator_t *generator, int jump) {
    return incr_pwd_char(generator, 0, jump);
}

/**
 * Frees a created pwd_generator_t
 * @param generator The generator to free
 */
void generate_pwd_free(pwd_generator_t *generator) {
    free(generator->pwd);
    free(generator->pwd_chars);
    free(generator);
}
