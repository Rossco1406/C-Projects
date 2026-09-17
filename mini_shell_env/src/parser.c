#include <stdio.h>
#include <string.h>
#include "shell.h"

void parse_input()
{
    my_shell.input[strcspn(my_shell.input, "\n")] = '\0';

    my_shell.argc = 0;

    char *token = strtok(my_shell.input, " ");

    while (token != NULL && my_shell.argc < MAXARGS - 1)
    {
        my_shell.argv[my_shell.argc] = token;
        my_shell.argc++;

        token = strtok(NULL, " ");
    }

    my_shell.argv[my_shell.argc] = NULL;
}