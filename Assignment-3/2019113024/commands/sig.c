#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "jobs.h"
#include "bexec.h"

char *signame[] = {"INVALID", "SIGHUP", "SIGINT", "SIGQUIT", "SIGILL", "SIGTRAP", "SIGABRT", "SIGBUS", "SIGFPE", "SIGKILL", "SIGUSR1", "SIGSEGV", "SIGUSR2", "SIGPIPE", "SIGALRM", "SIGTERM", "SIGSTKFLT", "SIGCHLD", "SIGCONT", "SIGSTOP", "SIGTSTP", "SIGTTIN", "SIGTTOU", "SIGURG", "SIGXCPU", "SIGXFSZ", "SIGVTALRM", "SIGPROF", "SIGWINCH", "SIGPOLL", "SIGPWR", "SIGSYS", NULL};

extern int n_childs;
extern struct childs childarr[1024];
struct childs temp[1024];

void sig(char *arr[])
{
    for (int i = 0; i < n_childs; i++)
    {
        temp[i].pid = childarr[i].pid;
        strcpy(temp[i].name, childarr[i].name);

        char path[256] = "";
        sprintf(path, "/proc/%d/stat", temp[i].pid);

        // printf("%s\n", path);
        FILE *file = fopen(path, "r");
        if (file != NULL)
        {
            char buff[256] = "";
            fscanf(file, "%*s %*s %s", buff);

            if (strcmp(buff, "R") == 0 | strcmp(buff, "S") == 0)
                temp[i].cur_Status = childarr[i].cur_Status = 1;
            if (strcmp(buff, "T") == 0)
                temp[i].cur_Status = childarr[i].cur_Status = 0;

            fclose(file);
        }
        else
        {
            temp[i].cur_Status = childarr[i].cur_Status;
        }
    }

    qsort(temp, n_childs, sizeof(struct childs), compare);

    int k = 0;
    while (arr[k] != NULL)
    {
        k++;
    }
    if (k == 3)
    {
        int signum = atoi(arr[2]);
        int job_id = atoi(arr[1]);
        if (job_id <= n_childs)
        {
            printf("Signal sent to %d\n", temp[job_id - 1].pid);
            if (kill(temp[job_id - 1].pid, signum) != 0)
                printf("Unable to end the signal to specified job\n");
        }
        else
            printf("job id is incorrect \n");
    }
    else
    {
        printf("Retarded syntax for sig --job_id --signum given \n");
    }
    // printf("signal passing\n");
}
