#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include "../utils.h"
void pinfo(char *arr[])
{
    pid_t pid = 0;
    if (arr[1] != NULL) // check for arg, if no arg use pid of parent process
        pid = atoi(arr[1]);
    else
        pid = getpid();
    int i = 0;
    while (arr[i++] != NULL)
        ;
    if (i > 3) // chek the no of args validity
    {
        printf("%d pinfo takes only 1 or no args\n", i);
        return;
    }
    // creating path to check /proc/$pid/stat and /proc/$pid/exe
    char s_pid[1024] = "";
    sprintf(s_pid, "%d", pid);
    char proc_path[1024] = "/proc/";
    char exe_path[1024] = "/proc/";

    strcat(proc_path, s_pid);
    strcat(exe_path, s_pid);

    strcat(proc_path, "/stat");
    strcat(exe_path, "/exe");

    FILE *file = fopen(proc_path, "r"); // open the stat file
    if (file != NULL)
    {
        char stats[1024] = "";
        fseek(file, 0, SEEK_SET);
        size_t val = fread(stats, 1024, 1, file); // read teh stat file
        fclose(file);
        char *c_arr[60];
        c_arr[0] = strtok(stats, " "); // tokeninze the stat buffer with " "
        int i = 0;
        while (c_arr[i] != NULL) // store the stats in an array
        {
            c_arr[++i] = strtok(NULL, " ");
        }
        char sign[10] = "";
        if (atoi(c_arr[4]) == atoi(c_arr[7])) // check if the process is running in foregorund to add +
            strcpy(sign, "+");
        // print the required insof about the process
        printf("pid -- %s\n", c_arr[0]);
        printf("Process Status -- %s%s\n", c_arr[2], sign);
        printf("memory -- %s\n", c_arr[22]);

        // get info abt the exe file path
        char link[1024] = "";
        int read_int = readlink(exe_path, link, 100);
        char buff[1024] = "";
        if (read_int != -1) // get the exe path
        {
            find_pwd(buff);
            get_path_from_home(buff, "", link);
            // if strcmp(buff, )
            printf("Executable Path -- %s\n", buff);
        }
        else
            printf("Executable Path -- %s\n", "absent");
    }
    else
    {
        printf("%d :invalid pid\n", pid);
    }
}
