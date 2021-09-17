#include <stdio.h>
#include "utils.h"
#include <signal.h>
#include <string.h>

int MAX_INP_LEN = 1024;
char PATH_CHD[1024];
char PATH_CWD[1024];
int main()
{
    clearscreen();
    find_pwd(PATH_CHD);
    find_hd(PATH_CHD);

    struct sigaction SIGAC;
    memset(&SIGAC, 0, sizeof(SIGAC));

    SIGAC.sa_flags = SA_RESTART;
    SIGAC.sa_handler = handlesignal;
    sigaction(SIGCHLD, &SIGAC, NULL);

    init_hist();
    while (1)
    {
        find_pwd(PATH_CWD);
        char INPUT_LINE[MAX_INP_LEN];
        printf("%s", getpromptline(PATH_CHD, PATH_CWD));
        fgets(INPUT_LINE, MAX_INP_LEN, stdin);
        process_input(INPUT_LINE);
    }
}