#include "cd.h"
#include "stdio.h"
#include <unistd.h>
#include <string.h>
#include "../utils.h"

char most_recent_dir[1024] = "~"; // variable to store the previous working directory for implementin cd -

void cd(char *arr[], char *home)
{
    // checkin if the no of parameters given to the cd command is less than 2.
    int l = 0;
    while (arr[l] != NULL)
        l++;
    if (l > 2)
    { // show the error message if arg count is invalid
        printf("cd can take only 1 or no args\n");
        return;
    }

    if (arr[1] != NULL) // take the necessary action if cd got an argument
    {
        if ((strcmp(arr[1], "..") == 0) | (strcmp(arr[1], ".") == 0)) // handling "." and ".." args to cd
        {
            getcwd(most_recent_dir, sizeof(most_recent_dir));
            chdir(arr[1]);
        }
        else if (arr[1][0] == '~') // handling all paths from home dir as arguments
        {
            char buff[1024] = "";
            strcpy(buff, home);
            strcat(buff, "/");
            char r_p[1024] = "";
            memcpy(r_p, &arr[1][1], strlen(arr[1]) - 1);
            strcat(buff, r_p);

            getcwd(most_recent_dir, sizeof(most_recent_dir));
            // printf("::%s\n", buff);
            chdir(buff);
        }
        else if (strcmp(arr[1], "-") == 0) // handling cd - ( go to previous dir)
        {
            char temp[1024];
            strcpy(temp, most_recent_dir);
            getcwd(most_recent_dir, sizeof(most_recent_dir));
            char path[1024];
            get_path_from_home(path, home, temp);
            printf("%s\n", path);
            chdir(temp);
        }
        else // handlind all the cases where a typical path is given as argument
        {
            getcwd(most_recent_dir, sizeof(most_recent_dir));
            int err = chdir(arr[1]);
            if (err != 0)
                perror(arr[1]); // handle error if cant cd into the given path
        }
    }
    else // handle case where cd got no argument
    {
        getcwd(most_recent_dir, sizeof(most_recent_dir));
        chdir(home);
    }
}