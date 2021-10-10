#ifndef FEXEC_H
#define FEXEC_H
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void fexec(char *arr[]);

struct FG
{
    pid_t pid;
    char name[1024];
};
struct FG current_fg;

#endif