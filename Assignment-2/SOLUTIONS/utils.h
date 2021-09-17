#ifndef UTILS_H
#define UTILS_H

void init_hist();
void clearscreen();

char *getpromptline(char *chd, char *cwd);
void find_hd(char *STR);
void find_pwd(char *STR);
void get_path_from_home(char *path, char *chd, char *cwd);
void execute_command(char *STR, int bg);
void process_input(char *input);
void handlesignal();
#endif