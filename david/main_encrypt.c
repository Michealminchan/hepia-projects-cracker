/**
 * Main program.
 * Crypt password with the provided salt in args.
 * David Gonzalez
 * 15.10.2016
 */


#include <stdio.h>
#include <stdlib.h>

#include "password.h"

#define MAIN_ARGS_COUNT 3

/**
 * Main function that treats input and output the crypted password.
 * @return Program status.
 */
int main(int argc, char * argv[])
{
    int exit_status = EXIT_SUCCESS;

    if (argc >= MAIN_ARGS_COUNT)
    {
        password_t pwd_data;
        password_init(&pwd_data);
        pwd_data.password = argv[1];
        pwd_data.salt = argv[2];

        password_crypt(&pwd_data);

        printf("%s\n", pwd_data.crypted_password);
    }
    else
    {
        printf("Usage: %s pwd_to_crypt used_salt\n", argv[0]);
        exit_status = EXIT_FAILURE;
    }

    return exit_status;
}
