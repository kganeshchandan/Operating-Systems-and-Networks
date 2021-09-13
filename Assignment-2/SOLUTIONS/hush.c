#include <stdio.h>
#include "utils.h"

int MAX_INP_LEN = 1024;
char PATH_CHD[1024];
char PATH_CWD[1024];
int main()
{
    clearscreen();
    find_pwd(PATH_CHD);
    find_pwd(PATH_CWD);
    while (1)
    {
        char INPUT_LINE[MAX_INP_LEN];
        printf("%s", getpromptline(PATH_CHD, PATH_CWD));
        fgets(INPUT_LINE, MAX_INP_LEN, stdin);
        process_input(INPUT_LINE);
    }
}