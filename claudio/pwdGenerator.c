#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "pwdGenerator.h"

static const int MAX_PWD_SIZE = 8;

static const char VALID_SYMBOLS[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
    'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '~', '!', '*'};

static int symbolCount = sizeof(VALID_SYMBOLS) / sizeof(VALID_SYMBOLS[0]);

static void initializePwdGenerator(pwdGenerator_t *generator) {
  generator->initialized = true;
  generator->pwd = (char *)malloc(sizeof(char) * (MAX_PWD_SIZE + 1));
  generator->pwdChars = (char *)malloc(sizeof(char) * MAX_PWD_SIZE);
  memset(generator->pwd, 0, MAX_PWD_SIZE + 1);
  memset(generator->pwdChars, -1, MAX_PWD_SIZE);
}

static char *pushPwdGeneratorForward(pwdGenerator_t *generator, int pos, int howMany) {

  int sum = (int)generator->pwdChars[pos] + howMany;
  int newChar = sum % symbolCount;
  generator->pwdChars[pos] = newChar;
  generator->pwd[pos] = VALID_SYMBOLS[newChar];
  int sumOverflow = sum / symbolCount;

  if (sumOverflow) {
    if (pos == MAX_PWD_SIZE - 1)
      return NULL;
    return pushPwdGeneratorForward(generator, pos + 1, sumOverflow);
  }

  return generator->pwd;
}

char *getNthPassword(int jump, pwdGenerator_t *generator) {

  if (!generator->initialized)
    initializePwdGenerator(generator);

  return pushPwdGeneratorForward(generator, 0, jump);
}