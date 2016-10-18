/**
 * Time measure module.
 * Some functions that help to measure execution time of a particular code.
 * Claudio Sousa, David Gonzalez
 * 18.10.2016
 */

#ifndef CRACK_H
#define CRACK_H

#include <stdbool.h>

char *crack(char *hash, char *salt, int threads);

#endif
