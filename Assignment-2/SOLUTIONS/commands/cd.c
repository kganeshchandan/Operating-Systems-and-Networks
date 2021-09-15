#include "cd.h"
#include "stdio.h"
#include <unistd.h>
#include <string.h>
#include "../utils.h"

char most_recent_dir[1024];
void cd(char *arr[], char *home)
{
    if (arr[1] != NULL)
    {
        printf("CD-ing to |%s|\n", arr[1]);
        if ((strcmp(arr[1], "..") == 0) | (strcmp(arr[1], ".") == 0))
        {
            getcwd(most_recent_dir, sizeof(most_recent_dir));
            chdir(arr[1]);
        }
        else if (strcmp(arr[1], "~") == 0)
        {
            getcwd(most_recent_dir, sizeof(most_recent_dir));
            chdir(home);
        }
        else if (strcmp(arr[1], "-") == 0)
        {
            char temp[1024];
            strcpy(temp, most_recent_dir);
            getcwd(most_recent_dir, sizeof(most_recent_dir));
            char path[1024];
            get_path_from_home(path, home, temp);
            printf("%s\n", path);
            chdir(temp);
        }
        else
        {
            char path[1024];
            getcwd(path, sizeof(path));
            strcat(path, "/");
            strcat(path, arr[1]);
            // printf("%s\n", path);
            getcwd(most_recent_dir, sizeof(most_recent_dir));
            int err = chdir(path);
            if (err != 0)
            {
                perror("Error encountered");
            }
        }
    }
    else
    {
        getcwd(most_recent_dir, sizeof(most_recent_dir));
        chdir(home);
    }
}