#ifndef CRACK_H
#define CRACK_H

#include <stdbool.h>

typedef struct
{
    char *pwd;
    bool found;
} crack_t;

crack_t *crack(char *hash, char *salt, int threads);

#endif
