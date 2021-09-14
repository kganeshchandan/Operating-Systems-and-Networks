#include <stdio.h>
#include <string.h>

void history(char **arr)
{
    int hash_arr[20] = {0};
    printf("history\n");
    int i, j = 19;
    while (i >= 0)
    {
        if (strcmp(arr + i, "empty") != 0)
        {
            hash_arr[i] = 1;
            break;
        }
        i--;
    }
    for (i; i < 20; i++)
    {
        if (hash_arr[i] == 1)
            printf(":%s\n", arr + i);
    }
}