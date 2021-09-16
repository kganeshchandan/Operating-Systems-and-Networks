#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void history(char *arr[], char *HIST_PATH)
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
void add_to_hist(char *input, char *HIST_PATH)
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