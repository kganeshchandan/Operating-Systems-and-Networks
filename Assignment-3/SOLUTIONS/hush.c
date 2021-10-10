#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int MAX_INP_LEN = 1024;
char PATH_CHD[1024];
char PATH_CWD[1024];
char BUFFER[1024];
pid_t parent_pid;

void launch_hush()
{

    printf("%d\n", getpid());
    while (1)
    {
        find_pwd(PATH_CWD);
        char INPUT_LINE[MAX_INP_LEN];
        printf("\033[1;32m");
        printf("%s", getpromptline(PATH_CHD, PATH_CWD));
        printf("\033[0m");
        if (fgets(INPUT_LINE, MAX_INP_LEN, stdin) != NULL)
            process_input(INPUT_LINE);
        else
        {
            printf("\n");
            exit(0);
        }
    }
}
int main()
{
    find_pwd(PATH_CHD);
    find_hd(PATH_CHD);

    struct sigaction SIGAC;
    memset(&SIGAC, 0, sizeof(SIGAC));

    SIGAC.sa_flags = SA_RESTART;
    SIGAC.sa_handler = handlesignal;
    sigaction(SIGCHLD, &SIGAC, NULL);

    signal(SIGINT, sigint_handler);
    signal(SIGTSTP, sigtstp_handler);

    init_hist();
    init_childs();
    // while (1)
    launch_hush();
}
