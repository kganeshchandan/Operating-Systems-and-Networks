#include "pwd.h"
#include "stdio.h"
#include "../utils.h"
#include <unistd.h>
void pwd(char *arr[], char *home)
{
    char path[256];
    get_path_from_home(path, home, getcwd(path, sizeof(path)));
    printf("%s\n", path);
}