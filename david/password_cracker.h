/**
 * Password cracker module.
 * With the help of the password module, efficiently crack password.
 * David Gonzalez
 * 15.10.2016
 */

#ifndef CRACKPWD_H
#define CRACKPWD_H

#include <stdlib.h>
#include <stdbool.h>

#include "time_measure.h"

/**
 * Structure used for the caller that set is wish.
 */
typedef struct password_cracker_param_t
{
    const char * crypted_password;  // Crypted password to crack.
    const char * salt;              // Used salt when crypted.
    const size_t nb_threads;        // Number of treads to use.

    const size_t min_length;        // Min length of password to crack.
    const size_t max_length;        // Max length of password to crack (does not include end char).
    const char * charset;           // Each character to be tested.

    const char ** dictionary;       // Complete file content, one password per line.
    const size_t dictionary_size;   // Number of element in the dictionary.

    char * cracked_password;        // Allocated buffer to put the cracked password. If not found, this will be empty.
    bool done_flag;                 // Tell if the "cracked_password" is filled. Must be init to false.
} password_cracker_param_t;

/**
 * Cracker params per thread
 */
typedef struct password_cracker_th_param_t
{
    size_t id;                          // Thread ID.
    password_cracker_param_t * params;  // Cracker params.
} password_cracker_th_param_t;

/**
 * Main function of cracking password. Use multi-threads.
 * @param params Parameters of the cracker.
 * @return TRUE if success, FALSE if failed.
 */
bool crack_pwd(password_cracker_param_t * params);

/**
 * Thread function that crack a password.
 * @param data Data structure with the password to crack and synchro informations.
 * @return None
 */
void * crack_pwd_th(void * data);

/**
 * Brute-force a crypted password.
 * @param params Parameters of the cracker.
 * @param offset Start offset in case of multithreading.
 * @param time_param Data for perf measurement. Can be NULL.
 */
void crack_pwd_bruteforce(password_cracker_param_t * params, size_t offset, time_measure_th_t * time_param);

/**
 * Use a loaded dictionary to crack a crypted password.
 * @param params Parameters of the cracker.
 * @param offset Start offset in case of multithreading.
 * @param time_param Data for perf measurement. Can be NULL.
 */
void crack_pwd_dictionary(password_cracker_param_t * params, size_t offset, time_measure_th_t * time_param);

#endif
