/**
 * Password module.
 * Helper functions for crypting password and checking crypted password validity.
 * David Gonzalez
 * 12.10.2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "password.h"

/**
 * Initialise a password data structure.
 * @param pwd_param Data of password to init.
 */
void password_init(password_t * pwd_param)
{
    pwd_param->cdata.initialized = 0;
    memset(pwd_param->crypted_password, '\0', PWD_CRYPTED_MAX);
}

/**
 * Encrypt a password.
 * @param pwd_param Data of password to crypt.
 */
void password_crypt(password_t * pwd_param)
{
    char * hash = crypt_r(pwd_param->password, pwd_param->salt, &pwd_param->cdata);

    if (hash != NULL)
    {
        memset(pwd_param->crypted_password, '\0', PWD_CRYPTED_MAX);
        strncpy(pwd_param->crypted_password, hash, PWD_CRYPTED_MAX);
        pwd_param->crypted_password[PWD_CRYPTED_MAX - 1] = '\0';
    }
    else
    {
        perror("Crypt password failed");
    }
}

/**
 * Take a password and verify that it is the right one, or not.
 * @param pwd_param Data of password to crypt.
 * @param against Crypted password to check against.
 * @return TRUE if match, FALSE otherwise.
 */
bool password_validate(password_t * pwd_param, const char * against)
{
    bool match = false;

    password_crypt(pwd_param);

    if (strncmp(pwd_param->crypted_password, against, PWD_CRYPTED_MAX) == 0)
    {
        match = true;
    }

    return match;
}
