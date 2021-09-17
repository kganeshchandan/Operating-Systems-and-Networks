#include "fexec.h"
#include "stdio.h"
#include "string.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void fexec(char *arr[])
{
    // printf("Executing in foreground\n");s

    int forkReturn = fork();
    // printf("BOTH\n");
    if (forkReturn == 0)
    {
        // forkReturn is ZERO for the child process.
        int ret = execvp(arr[0], arr);
        if (ret == -1)
            perror(arr[0]);
    }
    else
    {
        int wstatus = 0;
        pid_t ret = waitpid(forkReturn, &wstatus, WUNTRACED);
        // forkReturn is non-ZERO for the parent.
    }
}