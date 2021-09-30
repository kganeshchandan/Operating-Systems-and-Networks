#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "jobs.h"
#include "bexec.h"
#include "bg.h"

extern int n_childs;
extern struct childs childarr[1024];
struct childs temp[1024];

void bg(char *arr[])
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
    if (k == 2)
    {
        int job_id = atoi(arr[1]);
        if (job_id <= n_childs)
        {
            kill(temp[job_id - 1].pid, 18);
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
