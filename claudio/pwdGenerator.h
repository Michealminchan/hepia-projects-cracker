#ifndef PWDGENERATOR_H
#define PWDGENERATOR_H

#include <stdbool.h>

typedef struct
{
    bool initialized;
    char *pwd;
    char *pwdChars;
} pwdGenerator_t;

char * getNthPassword(int jump, pwdGenerator_t *generator);

#endif
