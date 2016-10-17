/**
 * Password module.
 * Helper functions for crypting password and checking crypted password validity.
 * David Gonzalez
 * 12.10.2016
 */

#ifndef PWD_H
#define PWD_H

#include <stdbool.h>
#include <crypt.h>

#define PWD_CRYPTED_MAX 256

typedef struct password_t
{
    struct crypt_data cdata;                // Data used for encryption.
    const char * password;                  // Password to crypt.
    const char * salt;                      // Salt used to crypt.
    char crypted_password[PWD_CRYPTED_MAX]; // Storage for encrypted password output.
} password_t;

/**
 * Initialise a password data structure.
 * @param pwd_param Data of password to init.
 */
void password_init(password_t * pwd_param);

/**
 * Encrypt a password.
 * @param pwd_param Data of password to crypt.
 */
void password_crypt(password_t * pwd_param);

/**
 * Take a password and verify that it is the right one, or not.
 * @param pwd_param Data of password to crypt.
 * @param against Crypted password to check against.
 * @return TRUE if match, FALSE otherwise.
 */
bool password_validate(password_t * pwd_param, const char * against);

#endif
