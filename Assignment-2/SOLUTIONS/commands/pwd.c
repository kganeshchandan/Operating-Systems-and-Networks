#include "pwd.h"
#include "stdio.h"
#include "../utils.h"
#include <unistd.h>

// using getcwd() library function to get the absolute path to current working directory.
void pwd(char *arr[], char *home)
{
    char path[1024]; // store the abs path given by getcwd() in path variable
    if (getcwd(path, sizeof(path)) != NULL)
        printf("%s\n", path);
    else // handle the error if the getcwd() fails to give the abs path
        perror("");
}