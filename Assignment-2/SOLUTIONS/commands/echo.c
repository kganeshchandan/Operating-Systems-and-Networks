#include "echo.h"
#include "stdio.h"
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