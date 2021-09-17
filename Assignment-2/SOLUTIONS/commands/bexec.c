#include "bexec.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void bexec(char *arr[])
{
    printf("running in BG\n");
    int forkReturn = fork();
    // printf("BOTH\n");
    int i = 0;
    // while (arr[i] != NULL)
    // {
    //     printf("::%s\n", arr[i]);
    //     i++;
    // }

    if (forkReturn == 0)
    {
        // forkReturn is ZERO for the child process.
        setpgid(0, 0);
        int ret = execvp(arr[0], arr);

        if (ret == -1)
            perror(arr[0]);
    }
    else
    {
        printf("%d\n", forkReturn);
        // forkReturn is non-ZERO for the parent.
    }
}
