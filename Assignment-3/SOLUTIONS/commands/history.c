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
    // check if the commands entered is a valid one to ush to history

    int i = 0;
    int bul = 0;
    for (i = 0; i < strlen(input) - 1; i++)
        if (input[i] != ' ' & input[i] != '\t')
            bul = 1;
    if (bul != 1)
        return;

    FILE *hist_file, *og_hist_file;

    og_hist_file = fopen(HIST_PATH, "r+"); // the next few lines see if the lates command in history matchse with the current command
    char og_ht[100005] = "";
    fseek(og_hist_file, 0, SEEK_END);
    int og_l = ftell(og_hist_file);
    fseek(og_hist_file, 0, SEEK_SET);
    fread(og_ht, 1, og_l, og_hist_file);
    char *og_token[30];
    og_token[0] = strtok(og_ht, "\n");
    i = 0;
    while (og_token[i] != NULL)
        og_token[++i] = strtok(NULL, "\n");
    fclose(og_hist_file);
    strcat(og_token[19], "\n");
    if (strcmp(og_token[19], input) == 0) // dont add to history if the input is same as latest command in history
        return;

    // adding the most recent command to history
    hist_file = fopen(HIST_PATH, "a");
    fprintf(hist_file, "%s", input);
    fclose(hist_file);

    hist_file = fopen(HIST_PATH, "r+");
    char history_text[100005] = "";

    fseek(hist_file, 0, SEEK_END);
    int l = ftell(hist_file);
    fseek(hist_file, 0, SEEK_SET);
    fread(history_text, 1, l, hist_file);
    // printf("%s", history_text);
    char *token[30];
    token[0] = strtok(history_text, "\n");
    i = 0;
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