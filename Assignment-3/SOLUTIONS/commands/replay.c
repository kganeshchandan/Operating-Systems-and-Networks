#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "../utils.h"

void replay(char *arr[])
{
    // printf("Entered Replay\n");
    int l = 0;
    int command_index = -1, interval_index = -1, period_index = -1;
    while (arr[l] != NULL)
    {
        if (strcmp(arr[l], "-period") == 0)
            period_index = l + 1;
        if (strcmp(arr[l], "-interval") == 0)
            interval_index = l + 1;
        if (strcmp(arr[l], "-command") == 0)
            command_index = l + 1;
        l++;
    }
    if (arr[period_index] != NULL & arr[interval_index] != NULL & arr[command_index] != NULL)
    {
        int interval = atoi(arr[interval_index]);
        int period = atoi(arr[period_index]);
        int time = 0;

        char command[1024] = "";
        for (int i = command_index; i < interval_index - 1; i++)
        {
            strcat(command, arr[i]);
            strcat(command, " ");
        }
        // printf("command to be executed: %s\n", command);

        while (time < period)
        {
            process_input(command);
            if (time + interval <= period)
            {
                sleep(interval);
                time = time + interval;
            }
            else
            {
                sleep(time + interval - period);
                time = time + time + interval - period;
            }
        }
    }
    else
    {
        printf("Invalid syntax for replay\n ");
    }
}