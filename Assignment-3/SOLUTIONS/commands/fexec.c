#include "fexec.h"
#include "stdio.h"
#include "string.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void fexec(char *arr[]) // running a process in foreground
{
    int forkReturn = fork(); // craeting a child to execture the foreground process and wait in parent process, till it finishes
    // printf("BOTH\n");
    if (forkReturn == 0)
    {
        // forkReturn is ZERO for the child process.
        int ret = execvp(arr[0], arr); // execute the command
        if (ret == -1)
            printf("%s :command not found\n", arr[0]);
    }
    else
    {
        int wstatus = 0;
        pid_t ret = waitpid(forkReturn, &wstatus, WUNTRACED); // wait till the foreground process finishes
        // forkReturn is non-ZERO for the parent.
    }
}