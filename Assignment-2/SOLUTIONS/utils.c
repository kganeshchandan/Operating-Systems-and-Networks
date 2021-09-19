#include "utils.h"
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

#include "commands/echo.h"
#include "commands/cd.h"
#include "commands/pwd.h"
#include "commands/repeat.h"
#include "commands/fexec.h"
#include "commands/bexec.h"
#include "commands/pinfo.h"
#include "commands/history.h"
#include "commands/ls.h"

char HOME_PATH[1024] = "";
char *HIST_ARR[20][1024];
char HIST_PATH[1024] = "/var/tmp/history.txt";
extern char PATH_CHD[1024];
extern char PATH_CWD[1024];
extern int MAX_INP_LEN;
extern void launch_hush();
struct childs childarr[1024];
extern int n_childs;

void init_hist()
{
    FILE *hist_file;
    hist_file = fopen(HIST_PATH, "r+");

    if (hist_file == NULL)
    {
        printf("histroy file doesnt exist, creating it\n");
        // fclose(hist_file);

        hist_file = fopen(HIST_PATH, "w+");
        fprintf(hist_file, "%s", "");
        fclose(hist_file);

        printf("craeted history file\n");
        hist_file = fopen(HIST_PATH, "a");
        for (int i = 0; i < 20; i++)
            fprintf(hist_file, "%s\n", "empty");
        fclose(hist_file);
    }
    else
        fclose(hist_file);
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
    if (strlen(cwd) >= strlen(HOME_PATH))
    {
        int k = 1;
        char new_path[256] = "~";
        for (int i = strlen(HOME_PATH); i < strlen(cwd); i++)
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
    if (username == NULL)
    {
        perror("unable to obtain username");
        exit(0);
    }

    strcat(username, "@");
    char host[256] = "";
    int hostname = gethostname(host, sizeof(host)); //find the host name

    if (hostname == -1)
    {
        perror("unable to obtain hostname");
        exit(0);
    }
    else
        strcat(username, host);
    strcat(username, ":)");

    char path[128];
    get_path_from_home(path, chd, cwd);
    strcat(username, path);

    strcat(username, "> ");
    // sprintf(username, "< %s", username);
    return username;
}

void execute_command(char *COMMAND, int bg)
{
    char *c_arr[10];
    int i = 0;

    c_arr[0] = strtok(COMMAND, " \t");
    // printf(":%s\n", c_arr[0]);

    if (c_arr[0] != NULL)
    {
        while (c_arr[i] != NULL)
        {
            // printf(":%s\n", c_arr[i]);
            c_arr[++i] = strtok(NULL, " \t");
        }
        // printf("last word%s\n", c_arr[i - 1]);
        if (strcmp(c_arr[0], "echo") == 0)
            echo(c_arr);
        else if (strcmp(c_arr[0], "cd") == 0)
            cd(c_arr, HOME_PATH);
        else if (strcmp(c_arr[0], "pwd") == 0)
            pwd(c_arr, HOME_PATH);
        else if (strcmp(c_arr[0], "history") == 0)
            history(c_arr, HIST_PATH);
        else if (strcmp(c_arr[0], "repeat") == 0)
            repeat(c_arr);
        else if (strcmp(c_arr[0], "pinfo") == 0)
            pinfo(c_arr);
        else if (strcmp(c_arr[0], "ls") == 0)
            ls(c_arr);
        else if (strcmp(c_arr[0], "exit") == 0)
            exit(0);
        else if (bg)
            bexec(c_arr);
        else
            fexec(c_arr);
    }
}
char str_and[1024] = "";

char *get_before_and(char *command)
{
    int hash_arr[1024] = {0};
    int len = strlen(command);
    int strt = 0, end = 0;
    char subtext[1024] = {'\0'};
    for (int i = 0; i < len; i++)
    {
        if (command[i] == '&')
        {
            end = i;
            memcpy(subtext, &command[strt], end - strt);
            subtext[end - strt] = '\0';
            strt = i + 1;
            strcat(subtext, " ");
            // printf(" go to bg |%s|\n", subtext);
            execute_command(subtext, 1);
        }
    }
    memcpy(subtext, &command[strt], len - strt);
    subtext[len - strt] = '\0';
    // strcat(subtext, " &");
    // printf("|%s|\n", subtext);
    execute_command(subtext, 0);
    // printf("%s\n", subtext);
    // printf("%d %d %d\n", len, strt, end);
}

void process_input(char *input)
{

    if (strlen(input) > MAX_INP_LEN)
    {
        printf(" Input len must be less than 1024\n");
        return;
    }
    char *arr[100];
    char *cp_arr[100];
    char hist_input[1024] = "";
    strcpy(hist_input, input);
    add_to_hist(hist_input, HIST_PATH);

    char copy_command[1024] = "";
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

        strcpy(copy_command, arr[j]);
        // printf("%d is %s|\n", j, copy_command);

        get_before_and(copy_command);
        // execute_command(arr[j]);
        j++;
    }
}

void handlesignal()
{
    char INPUT_LINE[MAX_INP_LEN];
    pid_t pid;
    int status;
    char pname[1024];
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        status = WIFEXITED(status);
        // printf("pid is %d\n", pid);
        for (int i = 0; i < n_childs; i++)
        {
            if (childarr[i].pid == pid)
            {
                strcpy(pname, childarr[i].name);
                printf("\nProcess %s with pid %d exited %s\n", pname, (int)pid, status != 0 ? "normally" : "abnormally");
                // childarr[i].pid = 0;

                printf("%s", getpromptline(PATH_CHD, PATH_CWD));
                fflush(stdout);
                // fgets("", MAX_INP_LEN, stdin);
                // process_input("");
                // printf("bruh\n");
            }
        }

        // break;
    }

    // launch_hush();
};
