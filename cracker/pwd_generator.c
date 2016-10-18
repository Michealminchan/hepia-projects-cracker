/**
 * Time measure module.
 * Some functions that help to measure execution time of a particular code.
 * Claudio Sousa, David Gonzalez
 * 18.10.2016
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pwd_generator.h"

static const int MAX_PWD_SIZE = 8;

static const char VALID_SYMBOLS[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~!*";
static const int SYMBOL_COUNT = sizeof(VALID_SYMBOLS) / sizeof(VALID_SYMBOLS[0]) - 1;

struct pwd_generator_t {
  char *pwd;
  char *pwd_chars; // VALID_SYMBOLS index of the chars in pwd
};

/**
 * Moves the password generator char at 'pos' by 'how_many'
 * @param generator The working generator
 * @param pos The index of the char to modify
 * @param how_many The step to increase the char
 * @return The new password
 */
static char *move_pwd_char(pwd_generator_t *generator, int pos, int how_many) {
  int sum = (int)generator->pwd_chars[pos] + how_many;
  int new_char = sum % SYMBOL_COUNT;
  generator->pwd_chars[pos] = new_char;
  generator->pwd[pos] = VALID_SYMBOLS[new_char];
  int sum_overflow = sum / SYMBOL_COUNT;

  if (sum_overflow) {
    if (pos == MAX_PWD_SIZE - 1)
      return NULL;
    return move_pwd_char(generator, pos + 1, sum_overflow);
  }
  return generator->pwd;
}

/**
 * Initializes a pwd_generator_t
 * @return The newly initialized pwd_generator_t
 */
pwd_generator_t *generate_pwd_init() {
  pwd_generator_t *generator = (pwd_generator_t *)malloc(sizeof(pwd_generator_t));
  generator->pwd = (char *)malloc(sizeof(char) * (MAX_PWD_SIZE + 1));
  generator->pwd_chars = (char *)malloc(sizeof(char) * MAX_PWD_SIZE);
  memset(generator->pwd, 0, MAX_PWD_SIZE + 1);
  memset(generator->pwd_chars, -1, MAX_PWD_SIZE);

  return generator;
}

/**
 * Moves the password generator to the next password
 * @param generator The working pwd generator
 * @param jump The nth pwd to jump to
 * @return The new password
 */
char *generate_pwd_next(pwd_generator_t *generator, int jump) { return move_pwd_char(generator, 0, jump); }

/**
 * Frees a created pwd_generator_t
 */
void generate_pwd_free(pwd_generator_t *generator) {
  free(generator->pwd);
  free(generator->pwd_chars);
  free(generator);
}
