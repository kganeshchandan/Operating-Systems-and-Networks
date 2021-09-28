#ifndef BEXEC_H
#define BEXEC_H
#include <unistd.h>
#include <stdio.h>
void bexec(char *arr[]);

struct childs
{
    pid_t pid;
    char name[1024];
    int cur_Status;
};
struct childs childarr[1024];
#endif