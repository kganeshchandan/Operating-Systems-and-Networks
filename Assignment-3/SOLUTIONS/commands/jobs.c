#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jobs.h"
#include "bexec.h"

extern int n_childs;
extern struct childs childarr[1024];

// extern childs;
struct childs temp[1024];

int compare(const void *a, const void *b)
{
    const struct childs *A = a;
    const struct childs *B = b;

    return (strcmp(A->name, B->name));
}
void jobs(char *arr[])
{
    for (int i = 0; i < n_childs; i++)
    {
        temp[i].pid = childarr[i].pid;
        strcpy(temp[i].name, childarr[i].name);
        temp[i].cur_Status = childarr[i].cur_Status;
    }

    // printf("entered jobs\n");
    qsort(temp, n_childs, sizeof(struct childs), compare);

    int k = 0;
    while (arr[k] != NULL)
    {
        k++;
    }
    if (k == 1)
    {
        for (int i = 0; i < n_childs; i++)
        {
            printf("[%d] %s %s [%d]\n", i + 1, temp[i].cur_Status == 0 ? "Stopped" : "Running", temp[i].name, temp[i].pid);
        }
    }
    else if (k > 1 & (strcmp(arr[1], "-r") == 0 | strcmp(arr[1], "-s") == 0))
    {
        for (int i = 0; i < n_childs; i++)
        {
            if (strcmp(arr[1], "-r") == 0 & temp[i].cur_Status == 1)
                printf("[%d] %s %s [%d]\n", i + 1, temp[i].cur_Status == 0 ? "Stopped" : "Running", temp[i].name, temp[i].pid);
            else if (strcmp(arr[1], "-s") == 0 & temp[i].cur_Status == 0)
                printf("[%d] %s %s [%d]\n", i + 1, temp[i].cur_Status == 0 ? "Stopped" : "Running", temp[i].name, temp[i].pid);
        }
    }
    else
        printf("Retarded syntax given for jobs -r/-s\n");
}