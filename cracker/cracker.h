/**
 * Password cracker module.
 * Bruteforce a password using multithreading.
 * Claudio Sousa, David Gonzalez
 * 22.10.2016
 */

#ifndef CRACK_H
#define CRACK_H

char *crack(char *hash, char *salt, int thread_count);

#endif
