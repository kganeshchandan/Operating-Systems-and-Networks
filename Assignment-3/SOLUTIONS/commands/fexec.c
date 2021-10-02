#include "fexec.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void fexec(char *arr[]) // running a process in foreground
{
    int forkReturn = fork(); // craeting a child to execture the foreground process and wait in parent process, till it finishes
    // printf("BOTH\n");
    // printf("The current pid is%d\n", getpid());
    if (forkReturn == 0)
    {
        // forkReturn is ZERO for the child process.
        // printf("The current pid of the process executing is %d\n", getpid());

        int ret = execvp(arr[0], arr); // execute the command
        if (ret == -1)
        {
            printf("%s :command not found\n", arr[0]);
            exit(0);
        }
        // printf("The current pid after executing is%d\n", getpid());
    }
    else
    {

        current_fg.pid = forkReturn;

        char input[1024] = "";
        int i = 0;
        while (arr[i] != NULL)
        {
            strcat(input, arr[i]);
            strcat(input, " ");
            i++;
        }
        strcpy(current_fg.name, input);
        int wstatus = 0;
        pid_t ret = waitpid(forkReturn, &wstatus, WUNTRACED); // wait till the foreground process finishes
        // forkReturn is non-ZERO for the parent.
    }
}