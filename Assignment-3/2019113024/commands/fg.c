#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <ctype.h>

#include "jobs.h"
#include "bexec.h"
#include "fexec.h"

#include "fg.h"
#include "bg.h"

extern int n_childs;
extern struct childs childarr[1024];
struct childs temp[1024];
extern struct FG current_fg;

void fg(char *arr[])
{
    bg(arr);
    // printf("entered fg\n");
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
    // printf("entered jobs\n");
    qsort(temp, n_childs, sizeof(struct childs), compare);

    int k = 0;
    while (arr[k] != NULL)
    {
        k++;
    }
    // printf("len = %d\n", k);
    if (k == 2)
    {
        if (atoi(arr[1]) <= n_childs)
        {
            int job_id = atoi(arr[1]);
            int p_pid = getpid();
            current_fg.pid = temp[job_id - 1].pid;
            strcpy(current_fg.name, temp[job_id - 1].name);

            for (int i = 0; i < n_childs; i++)
            {
                if (childarr[i].pid == current_fg.pid)
                {
                    // printf("yeeting %d\n", childarr[i].pid);

                    for (int j = i; j < n_childs - 1; j++)
                    {
                        childarr[j].pid = childarr[j + 1].pid;
                        strcpy(childarr[j].name, childarr[j + 1].name);
                    }
                    n_childs--;
                }
            }
            // setpgid(current_fg.pid, getpid());

            signal(SIGTTOU, SIG_IGN);
            signal(SIGTTIN, SIG_IGN);
            tcsetpgrp(0, getpgid(current_fg.pid));

            // waitpid(current_fg.pid, NULL, WUNTRACED);
            int status;
            // printf("fg process rn %d\n", current_fg.pid);
            fflush(stdout);

            if (waitpid(current_fg.pid, &status, WUNTRACED))
            {
                int stat = WIFSTOPPED(status);

                // printf("killing\n");
                if (WIFSTOPPED(status))
                {
                    // printf("killed\n");
                    childarr[n_childs].pid = current_fg.pid;
                    strcpy(childarr[n_childs].name, current_fg.name);
                    n_childs++;
                }
                fflush(stdout);
            }

            tcsetpgrp(0, getpgid(0));
            signal(SIGTTOU, SIG_DFL);
            signal(SIGTTIN, SIG_DFL);

            // printf("gg\n");
            // kill(current_fg.pid, 19);
        }
        else
        {
            printf("Retarded  syntax given\n");
        }
    }
    else
        printf("Retarded  syntax given\n");
}