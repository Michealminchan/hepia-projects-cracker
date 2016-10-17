#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "pwdGenerator.h"

const int MAX_PWD_SIZE = 8;

const char VALID_SYMBOLS[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c',
    'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C',
    'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '~', '!', '*'};

int symbolCount = sizeof(VALID_SYMBOLS) / sizeof(VALID_SYMBOLS[0]);

void initializePwdGenerator(pwdGenerator_t *generator) {
  generator->initialized = true;
  generator->pwd = (char *)malloc(sizeof(char) * (MAX_PWD_SIZE + 1));
  generator->pwdChars = (char *)malloc(sizeof(char) * MAX_PWD_SIZE);
  for (int i = 0; i < MAX_PWD_SIZE; i++) {
    generator->pwdChars[i] = -1;
    generator->pwd[0] = 0;
  }
}

char *getNthPassword(int jump, pwdGenerator_t *generator) {
  if (!generator->initialized)
    initializePwdGenerator(generator);

  int charPos = -1;
  while (jump > 0) {
    charPos++;
    if (charPos >= MAX_PWD_SIZE)
      return NULL;
    char charDelta = jump % symbolCount;
    jump /= symbolCount;
    if (charDelta == 0)
      continue;
    int newChar = generator->pwdChars[charPos] + charDelta;
    if (newChar >= symbolCount) {
      jump += newChar / symbolCount;
      newChar %= symbolCount;
    }
    generator->pwdChars[charPos] = newChar;
    generator->pwd[charPos] = VALID_SYMBOLS[newChar];
  }
  if (charPos > 3) {
    if (generator->pwd[0] == '0' && generator->pwd[1] == '0' &&
        generator->pwd[2] == '0' && generator->pwd[3] == '0')
      printf("%s\n", generator->pwd);
  }
  //printf("%s\n", generator->pwd);
  return generator->pwd;
}