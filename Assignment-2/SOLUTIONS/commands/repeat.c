#include <stdio.h>
#include "../utils.h"
#include <stdlib.h>
#include <string.h>
void repeat(char *arr[])
{
    int k = 0;
    while (arr[k++] != NULL)
        ;
    if (k == 3 | k == 2)
    {
        printf("Please enter -<num> -<command> as the args to repeat command\n");
        return;
    }
    int n = atoi(arr[1]);
    char new_commad[1024] = "";
    int i = 2;
    while (arr[i] != NULL)
    {
        strcat(new_commad, arr[i]);
        strcat(new_commad, " ");
        i++;
    }
    while (n--)
    {

        char temp[1024] = "";
        strcpy(temp, new_commad);
        execute_command(temp, 0);
    }
}