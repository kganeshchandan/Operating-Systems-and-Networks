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
#include <fcntl.h>

#include "commands/echo.h"
#include "commands/cd.h"
#include "commands/pwd.h"
#include "commands/repeat.h"
#include "commands/fexec.h"
#include "commands/bexec.h"
#include "commands/pinfo.h"
#include "commands/history.h"
#include "commands/ls.h"
#include "commands/jobs.h"
#include "commands/sig.h"
#include "commands/bg.h"
#include "commands/fg.h"
#include "commands/replay.h"

char HOME_PATH[1024] = "";
char *HIST_ARR[20][1024];
char HIST_PATH[1024] = "/var/tmp/history.txt";
extern char PATH_CHD[1024];
extern char PATH_CWD[1024];
extern int MAX_INP_LEN;
extern void launch_hush();
extern struct childs childarr[1024];
extern int n_childs;
extern struct FG current_fg;
extern pid_t parent_pid;

void init_childs()
{
    for (int i = 0; i < 1024; i++)
    {
        childarr[i].pid = -1;
        strcpy(childarr[i].name, "None");
        childarr[i].cur_Status = -1;
    }
}
void sigint_handler(int signum)
{
    printf("\n");
    printf("%d lol \n", current_fg.pid);
    if (current_fg.pid == 0 | current_fg.pid == getpid())
        printf("\033[1;32m%s\033[0m", getpromptline(PATH_CHD, PATH_CWD));

    fflush(stdout);
    current_fg.pid = getpid();
    return;
}

void sigtstp_handler(int signum)
{
    printf("\n");
    // printf("%s with pid %d is pushed to bg\n", current_fg.name, current_fg.pid);
    printf("\033[1;32m%s\033[0m", getpromptline(PATH_CHD, PATH_CWD));
    fflush(stdout);

    // childarr[n_childs].pid = current_fg.pid;
    // strcpy(childarr[n_childs].name, current_fg.name);
    // childarr[n_childs].cur_Status = 1;
    // n_childs++;
    // return;
}

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

int check_redirections(char *command)
{
    char *words[10];
    words[0] = strtok(command, " \t");
    int i = 0;
    if (words[0] != NULL)
    {
        while (words[i] != NULL)
            words[++i] = strtok(NULL, " \t");
        /////////////////////////////////////////////////////////////////
        int prev_marker = 0;
        // printf("len %d\n", i);
        int hash_arr[2] = {0, 0};
        int ifd, ofd, og_ifd, og_ofd;
        for (int k = 0; k < i; k++)
        {
            if (strcmp(words[k], "<") == 0)
            {
                // printf("bruh\n");
                ifd = open(words[k + 1], O_RDONLY);
                og_ifd = dup(STDIN_FILENO);
                dup2(ifd, STDIN_FILENO);
                // printf("bruh\n");

                words[k] = NULL;
                hash_arr[0] = 1;
                prev_marker = k;
            }
            else if (strcmp(words[k], ">") == 0)
            {
                ofd = open(words[k + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                og_ofd = dup(STDOUT_FILENO);
                dup2(ofd, STDOUT_FILENO);

                words[k] = NULL;
                hash_arr[1] = 1;
                prev_marker = k;
            }
            else if (strcmp(words[k], ">>") == 0)
            {
                ofd = open(words[k + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                og_ofd = dup(STDOUT_FILENO);
                dup2(ofd, STDOUT_FILENO);

                words[k] = NULL;
                hash_arr[1] = 1;
                prev_marker = k;
            }
        }
        int l = 0;
        char newcommand[1024] = "";
        while (words[l] != NULL)
        {
            strcat(newcommand, words[l]);
            strcat(newcommand, " ");
            l++;
        }
        if (prev_marker > 0)
        {
            execute_command(newcommand, 0);
            if (hash_arr[0] == 1)
            {
                close(ifd);
                dup2(og_ifd, STDIN_FILENO);
            }
            if (hash_arr[1] == 1)
            {
                close(ofd);
                dup2(og_ofd, STDOUT_FILENO);
            }
            return 1;
        }
        else
            return 0;
    }
}

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

void handle_pipes(char *COMMAND)
{
    int n_pipes = 0;
    char *c_arr[100];
    c_arr[0] = strtok(COMMAND, "|");
    while (c_arr[n_pipes] != NULL)
        c_arr[++n_pipes] = strtok(NULL, "|");
    n_pipes--;
    int og_stdout = STDOUT_FILENO;
    int og_stdin = STDIN_FILENO;

    if (n_pipes == 0)
    {
        // printf("bruh\n");
        get_before_and(c_arr[0]);
        // return;
    }
    else
    {
        int all_fds[2 * n_pipes + 1];
        for (int k = 0; k < n_pipes; k++)
        {
            if (pipe(all_fds + 2 * k) == -1)
                printf("pipe Error\n");
        }
        int pid;
        for (int i = 0; i < n_pipes + 1; i++)
        {
            pid = fork();
            if (pid == 0)
            {
                if (i < n_pipes)
                    dup2(all_fds[2 * i + 1], STDOUT_FILENO);

                if (i > 0)
                    dup2(all_fds[2 * (i - 1)], STDIN_FILENO);

                for (int k = 0; k < 2 * n_pipes; k++)
                    close(all_fds[k]);

                char temp[1024] = "";
                strcpy(temp, c_arr[i]);
                get_before_and(temp);
                exit(0);
            }
        }
        for (int k = 0; k < 2 * n_pipes; k++)
            close(all_fds[k]);
        for (int k = 0; k < n_pipes + 1; k++)
            wait(NULL);
    }
}

void execute_command(char *COMMAND, int bg_bool)
{
    // printf("command given to execute|%s|\n", COMMAND);
    char temp[1024] = "";
    strcpy(temp, COMMAND);
    if (check_redirections(temp) != 0)
    {
        // printf("Found redirections\n");
        return;
    }

    char *c_arr[10];
    int i = 0;

    c_arr[0] = strtok(COMMAND, " \t");

    // printf(":%s\n", c_arr[0]);
    if (c_arr[0] != NULL)
    {
        while (c_arr[i] != NULL)
            c_arr[++i] = strtok(NULL, " \t");
        // printf("last word%s\n", c_arr[i - 1]);

        if (strcmp(c_arr[0], "echo") == 0)
            echo(c_arr);
        else if (bg_bool)
            bexec(c_arr);
        else if (strcmp(c_arr[0], "replay") == 0)
            replay(c_arr);
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
        else if (strcmp(c_arr[0], "jobs") == 0)
            jobs(c_arr);
        else if (strcmp(c_arr[0], "sig") == 0)
            sig(c_arr);
        else if (strcmp(c_arr[0], "bg") == 0)
            bg(c_arr);
        else if (strcmp(c_arr[0], "fg") == 0)
            fg(c_arr);
        else
            fexec(c_arr);
    }
}

char str_and[1024] = "";

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
        handle_pipes(copy_command);
        // get_before_and(copy_command);
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
        // printf("\npid is just finished %d\n", pid);

        for (int i = 0; i < n_childs; i++)
        {
            if (childarr[i].pid == pid)
            {
                strcpy(pname, childarr[i].name);
                printf("\nProcess %s with pid %d exited %s\n", pname, (int)pid, status != 0 ? "normally" : "abnormally");
                printf("\033[1;32m%s\033[0m", getpromptline(PATH_CHD, PATH_CWD));
                fflush(stdout);
                for (int j = i; j < n_childs - 1; j++)
                {
                    childarr[j].pid = childarr[j + 1].pid;
                    strcpy(childarr[j].name, childarr[j + 1].name);
                }
                n_childs--;
            }
        }
        // for (int i = 0; i < n_childs; i++)
        // {
        //     printf("\npid = %d\n", childarr[i].pid);
        // }

        // break;
    }

    // launch_hush();
};
