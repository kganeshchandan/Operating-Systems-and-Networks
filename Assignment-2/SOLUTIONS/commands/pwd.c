#include "pwd.h"
#include "stdio.h"
#include "../utils.h"
#include <unistd.h>
void pwd(char *arr[], char *home)
{
    char path[256];
    getcwd(path, sizeof(path));
    printf("%s\n", path);
}