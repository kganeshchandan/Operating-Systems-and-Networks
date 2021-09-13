#include "utils.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

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

void process_input(char *input)
{
    printf("%s", input);
}