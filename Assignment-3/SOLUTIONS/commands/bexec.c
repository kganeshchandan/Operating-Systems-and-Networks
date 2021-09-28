#include "bexec.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// extern struct childs childarr[1024];

int n_childs = 0;
void bexec(char *arr[]) // func to run the command in bg
{
    // printf("running in BG\n");
    int forkReturn = fork(); // craet a child to run the bg process
    int i = 0;
    if (forkReturn == 0)
    {
        // forkReturn is ZERO for the child process.
        setpgid(0, 0); // give the child a grpid of pid
        int ret = execvp(arr[0], arr);

        if (ret == -1)
        {
            printf("\n%s :command not found\n", arr[0]);
            return;
        }
    }
    else
    {
        printf("%d\n", forkReturn); // print the process id of the bg process

        childarr[n_childs].pid = forkReturn; // maintain a list of child processes
        char input[1024] = "";
        i = 0;
        while (arr[i] != NULL)
        {
            strcat(input, arr[i]);
            strcat(input, " ");
            i++;
        }
        strcpy(childarr[n_childs].name, input);
        childarr[n_childs].cur_Status = 1;
        n_childs++;
    }
}
