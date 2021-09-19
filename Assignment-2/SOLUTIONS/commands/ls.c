#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

extern char HOME_PATH[1024];

char fperms[10] = "----------";

char *get_perms(mode_t bits) // a function to get the permisions  string for -l flag from the bit map
{
    char perms[11] = "----------";
    if (S_ISREG(bits))
        perms[0] = '-';
    else if (S_ISDIR(bits))
        perms[0] = 'd';
    else if (S_ISCHR(bits))
        perms[0] = 'c';
    else if (S_ISBLK(bits))
        perms[0] = 'b';
    else if (S_ISFIFO(bits))
        perms[0] = 'f';
    else if (S_ISLNK(bits))
        perms[0] = 'l';
    else if (S_ISSOCK(bits))
        perms[0] = 's';

    if (bits & S_IRUSR)
        perms[1] = 'r';
    if (bits & S_IWUSR)
        perms[2] = 'w';
    if (bits & S_IXUSR)
        perms[3] = 'x';
    if (bits & S_IRGRP)
        perms[4] = 'r';
    if (bits & S_IWGRP)
        perms[5] = 'w';
    if (bits & S_IXGRP)
        perms[6] = 'x';
    if (bits & S_IROTH)
        perms[7] = 'r';
    if (bits & S_IWOTH)
        perms[8] = 'w';
    if (bits & S_IXOTH)
        perms[9] = 'x';

    strcpy(fperms, perms);

    return fperms;
}

void print_stat(struct stat stats, char *name) // func to print the file info for -l flag
{
    char line[1024] = "";
    strcat(line, get_perms(stats.st_mode)); //get permissions
    strcat(line, " ");

    int nlink = stats.st_nlink;
    char _nlink[10] = "";
    sprintf(_nlink, "%d", nlink); // get the no of links
    strcat(line, _nlink);
    strcat(line, " ");

    strcat(line, getpwuid(stats.st_uid)->pw_name); // get the user id
    strcat(line, " ");

    strcat(line, getgrgid(stats.st_uid)->gr_name); // get the group id
    strcat(line, " ");

    // strcat(line, ctime(&stats.st_mtime));
    char buffer[80] = "";
    strftime(buffer, 80, "%b %d %H:%M", localtime(&stats.st_mtime)); // get the last modified time & format it
    strcat(line, buffer);
    strcat(line, " ");
    strcat(line, name);
    printf("%s\n", line);
}

void ls(char *arr[]) //implementing ls
{
    char *path_arr[10] = {'\0'}; // creating an array of strings to store all the path arguments to ls
    int flag_arr[2] = {0, 0};    //  creating a hash array for -a -l flags, [a flag, l flag]

    int i = 1, j = 0;
    // marking all flags in flag arr
    // printf("%s\n", arr[1][0]);
    while (arr[i] != NULL) // iterate over all the arguments to identify flags and paths separtely
    {
        if ((strcmp(arr[i], "-a") == 0)) // idenitfy -a flag
            flag_arr[0] = 1;
        else if ((strcmp(arr[i], "-l") == 0)) // idenitfy -l flag
            flag_arr[1] = 1;
        else if ((strcmp(arr[i], "-al") == 0) | (strcmp(arr[i], "-la") == 0)) // identify -al -la flags
            flag_arr[0] = flag_arr[1] = 1;
        else
        {
            if (arr[i][0] == '~') // identifying args which start with ~ and put it in path_arr
            {
                char buff[1024] = "";
                strcpy(buff, HOME_PATH);
                strcat(buff, "/");
                char r_p[1024] = "";
                memcpy(r_p, &arr[i][1], strlen(arr[i]) - 1);
                strcat(buff, r_p);
                path_arr[j++] = buff;
            }
            else
                path_arr[j++] = arr[i];
        }

        i++;
    }
    if (path_arr[0] == NULL) // if no args r given to ls it should be same as ls .
    {
        path_arr[0] = ".";
        j = 1;
    }

    int path_len = j;

    int item;
    for (item = 0; item < path_len; item++) // iterate over all paths given to show the ls of each one
    {
        DIR *d;
        struct dirent *dir;
        if (path_len > 1)
            printf("%s:\n", path_arr[item]);

        d = opendir(path_arr[item]);
        char *dir_content[1024];  // store all the files in a dir here
        char file_name[256] = ""; // filename of arg
        int no_of_files = 0;      // no of files in each arg

        if (d) // if the dir path given is valid
        {
            i = 0;
            while ((dir = readdir(d)) != NULL) // iterativly add files to dir content
            {
                dir_content[i] = (char *)malloc(strlen(dir->d_name) + 1);
                strcpy(dir_content[i], dir->d_name);
                // printf("%s\n", dir_content[i]);
                i++;
            }
            no_of_files = i;
            closedir(d);
            char temp_str[256] = "";

            if ((flag_arr[0] == 0) & (flag_arr[1] == 0)) // no flags r present
            {
                for (int k = 0; k < no_of_files; k++)
                {
                    strcpy(temp_str, dir_content[k]);
                    if (temp_str[0] != '.')
                        printf("%s ", temp_str);
                }
                printf("\n");
            }

            if ((flag_arr[0] == 1) & (flag_arr[1] == 0)) // only -a flag is present
            {
                for (int k = 0; k < no_of_files; k++)
                    printf("%s ", dir_content[k]);
                printf("\n");
            }

            if ((flag_arr[0] == 0) & (flag_arr[1] == 1)) // only -l flag is present
            {
                blkcnt_t size = 0;
                for (int j = 0; j < no_of_files; j++)
                {
                    struct stat stats;
                    char path_to_file[1024] = "";
                    strcpy(path_to_file, path_arr[item]);
                    strcat(path_to_file, "/");
                    strcat(path_to_file, dir_content[j]);
                    // printf("%s\n", path_to_file);
                    strcpy(temp_str, dir_content[j]);
                    if (temp_str[0] != '.')
                    {
                        lstat(path_to_file, &stats);
                        size = size + stats.st_blocks;
                        // printf("total %ld\n", size / 2);
                    }
                }
                printf("total %ld\n", size / 2);
                for (int j = 0; j < no_of_files; j++)
                {
                    struct stat stats;
                    char path_to_file[1024] = "";
                    strcpy(path_to_file, path_arr[item]);
                    strcat(path_to_file, "/");
                    strcat(path_to_file, dir_content[j]);
                    // printf("%s\n", path_to_file);
                    strcpy(temp_str, dir_content[j]);
                    if (temp_str[0] != '.')
                    {
                        if (lstat(path_to_file, &stats) == 0)
                            print_stat(stats, dir_content[j]);
                    }
                }
            }

            if ((flag_arr[0] == 1) & (flag_arr[1] == 1)) // both -a -l flags r present
            {
                blkcnt_t size = 0;
                for (int j = 0; j < no_of_files; j++)
                {
                    struct stat stats;
                    char path_to_file[1024] = "";
                    strcpy(path_to_file, path_arr[item]);
                    strcat(path_to_file, "/");
                    strcat(path_to_file, dir_content[j]);
                    // printf("%s\n", path_to_file);
                    lstat(path_to_file, &stats);
                    size = size + stats.st_blocks;
                    // printf("total %ld\n", size / 2);
                }
                printf("total %ld\n", size / 2);
                for (int j = 0; j < no_of_files; j++)
                {
                    struct stat stats;
                    char path_to_file[1024] = "";
                    strcpy(path_to_file, path_arr[item]);
                    strcat(path_to_file, "/");
                    strcat(path_to_file, dir_content[j]);
                    // printf("%s\n", path_to_file);
                    if (lstat(path_to_file, &stats) == 0)
                        print_stat(stats, dir_content[j]);
                }
            }
        }
        else // path given is incorrect
        {
            perror(path_arr[item]);
        }
        // printf("\n");
    }
}
