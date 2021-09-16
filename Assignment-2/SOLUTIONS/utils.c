#include "utils.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands/echo.h"
#include "commands/cd.h"
#include "commands/pwd.h"
#include "commands/repeat.h"
#include "commands/fexec.h"
#include "commands/bexec.h"
#include "commands/pinfo.h"
// #include "commands/history.h"

char HOME_PATH[1024] = "";
char *HIST_ARR[20][1024];
char HIST_PATH[1024] = "/var/tmp/history.txt";

void history(char *arr[])
{
    // char HIST_PATH[1024] = "";
    // strcat(HIST_PATH, HOME_PATH);
    // strcat(HIST_PATH, "/history.txt");

    FILE *hist_file;
    hist_file = fopen(HIST_PATH, "r+");
    char history_text[100005] = "";

    fseek(hist_file, 0, SEEK_END);
    int l = ftell(hist_file);
    fseek(hist_file, 0, SEEK_SET);
    fread(history_text, 1, l, hist_file);

    char *token[30], *new_token[30];
    token[0] = strtok(history_text, "\n");
    int i = 0;
    while (token[i] != NULL)
    {
        // printf("::%s\n", token[i]);
        token[++i] = strtok(NULL, "\n");
    }
    fclose(hist_file);

    int num = 10;
    if (arr[1] != NULL)
        num = atoi(arr[1]);
    if ((num < 21) & (num > 0))
    {
        for (int i = 20 - num; i < 20; i++)
        {
            if (strcmp(token[i], "empty") != 0)
                printf("%s\n", token[i]);
        }
    }
    else
    {
        printf(" --<num> argument must be integer from 1 to 20\n");
    }
}

void init_hist()
{
    // char HIST_PATH[1024] = "";
    // strcat(HIST_PATH, HOME_PATH);
    // strcat(HIST_PATH, "/history.txt");

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

void add_to_hist(char *input)
{
    // char HIST_PATH[1024] = "";
    // strcat(HIST_PATH, HOME_PATH);
    // strcat(HIST_PATH, "/history.txt");

    FILE *hist_file;
    hist_file = fopen(HIST_PATH, "a");
    char cpy_inp[1024];
    strcpy(cpy_inp, input);
    fprintf(hist_file, "%s", input);
    fclose(hist_file);

    hist_file = fopen(HIST_PATH, "r+");
    char history_text[100005] = "";

    fseek(hist_file, 0, SEEK_END);
    int l = ftell(hist_file);
    fseek(hist_file, 0, SEEK_SET);
    fread(history_text, 1, l, hist_file);
    // printf("%s", history_text);
    char *token[30], *new_token[30];
    token[0] = strtok(history_text, "\n");
    int i = 0;
    while (token[i] != NULL)
    {
        // printf("::%s\n", token[i]);
        token[++i] = strtok(NULL, "\n");
    }
    fclose(hist_file);
    hist_file = fopen(HIST_PATH, "w");
    fprintf(hist_file, "%s", "");
    fclose(hist_file);

    char buffer[10005] = "";
    int j = 1;
    // printf(" i is %d\n", i);
    hist_file = fopen(HIST_PATH, "a");
    while (j < i)
    {
        fprintf(hist_file, "%s\n", token[j]);
        j++;
    }

    fclose(hist_file);
}

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
        else if ((i > 1) & (strcmp(c_arr[i - 1], "&") == 0))
        {
            bexec(c_arr);
        }
        else if (strcmp(c_arr[0], "history") == 0)
            history(c_arr);
        else if (strcmp(c_arr[0], "repeat") == 0)
            repeat(c_arr);
        else if (strcmp(c_arr[0], "pinfo") == 0)
            pinfo(c_arr);
        else
            fexec(c_arr);
    }
}

void process_input(char *input)
{

    char *arr[10];
    char hist_input[1024] = "";
    strcpy(hist_input, input);
    add_to_hist(hist_input);
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
