#include "echo.h"
#include "stdio.h"

// implement echo terminal command by just printing the input command
///by removing all spaces and tabs
void echo(char *arr[])
{
    int i = 1;
    while (arr[i] != NULL)
    {
        printf("%s ", arr[i]);
        i++;
    }
    printf("\n");
}