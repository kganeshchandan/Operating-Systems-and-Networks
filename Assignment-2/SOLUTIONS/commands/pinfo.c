#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include "../utils.h"
void pinfo(char *arr[])
{
    pid_t pid = 0;
    if (arr[1] != NULL)
        pid = atoi(arr[1]);
    else
        pid = getpid();

    char s_pid[1024] = "";
    sprintf(s_pid, "%d", pid);
    char proc_path[1024] = "/proc/";
    char exe_path[1024] = "/proc/";

    strcat(proc_path, s_pid);
    strcat(exe_path, s_pid);

    strcat(proc_path, "/stat");
    strcat(exe_path, "/exe");

    // printf("%s\n", proc_path);
    FILE *file = fopen(proc_path, "r");
    if (file != NULL)
    {
        char stats[1024] = "";
        fseek(file, 0, SEEK_SET);
        size_t val = fread(stats, 1024, 1, file);
        fclose(file);
        char *c_arr[60];
        c_arr[0] = strtok(stats, " ");
        int i = 0;
        while (c_arr[i] != NULL)
        {
            c_arr[++i] = strtok(NULL, " ");
        }
        char sign[10] = "";
        // printf("%d, %d\n", atoi(c_arr[4]), atoi(c_arr[7]));
        if (atoi(c_arr[4]) == atoi(c_arr[7]))
            strcpy(sign, "+");
        printf("pid --%s\n", c_arr[0]);
        printf("Process Status --%s%s\n", c_arr[2], sign);
        printf("memory --%s\n", c_arr[22]);
        char link[1024] = "";
        int read_int = readlink(exe_path, link, 100);
        char buff[1024] = "";
        if (read_int != -1)
        {
            find_pwd(buff);
            get_path_from_home(buff, "", link);
            // if strcmp(buff, )
            printf("Executable Path --%s\n", buff);
        }
        else
            printf("Executable Path --%s\n", "absent");
    }
    else
    {
        perror("invalid pid");
    }
}
