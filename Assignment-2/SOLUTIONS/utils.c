#include "utils.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "commands/echo.h"
#include "commands/cd.h"

char HOME_PATH[1024] = "";

void clearscreen()
{
}

void error_handle(int error)
{
    perror("Error");
}

void find_pwd(char *STR)
{
    char temp[100];
    getcwd(temp, sizeof(temp));
    strcpy(STR, temp);
}
void find_hd(char *STR)
{
    getcwd(HOME_PATH, sizeof(HOME_PATH));
    strcpy(STR, HOME_PATH);
}

void get_path_from_home(char *path, char *chd, char *cwd)
{
    if (strlen(cwd) >= strlen(chd))
    {
        int k = 1;
        char new_path[256] = "~";
        for (int i = strlen(chd); i < strlen(cwd); i++)
        {
            new_path[k] = cwd[i];
            k++;
        }
        strcpy(path, new_path);
    }
    else
    {
        strcpy(path, cwd);
    }
}
char *getpromptline(char *chd, char *cwd)
{
    char *username = getlogin();
    strcat(username, "@");
    char host[256] = "";
    int hostname = gethostname(host, sizeof(host)); //find the host name

    if (hostname == -1)
        error_handle(hostname);
    else
        strcat(username, host);
    strcat(username, ":)");

    char path[128];
    get_path_from_home(path, chd, cwd);
    strcat(username, path);

    strcat(username, "> ");
    return username;
}

void execute_command(char *COMMAND)
{
    char *c_arr[10];
    int i = 0;

    c_arr[0] = strtok(COMMAND, " \t");
    // printf(":%s\n", c_arr[0]);

    if (c_arr[0] != NULL)
    {
        while (c_arr[i] != NULL)
        {
            // printf(":%s", c_arr[0]);
            c_arr[++i] = strtok(NULL, " \t");
        }

        if (strcmp(c_arr[0], "echo") == 0)
            echo(c_arr);
        if (strcmp(c_arr[0], "cd") == 0)
            cd(c_arr, HOME_PATH);
    }
}

void process_input(char *input)
{
    char *arr[10];
    int i = 0;
    arr[0] = strtok(input, ";\n");
    while (arr[i] != NULL)
    {
        i = i + 1;
        // printf("%s\n", arr[i]);
        arr[i] = strtok(NULL, ";\n");
    }
    int j = 0;
    while (arr[j] != NULL)
    {
        // printf(":::::::;;%s|\n", arr[j]);

        execute_command(arr[j]);
        j++;
    }
}
