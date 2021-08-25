#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
int yesorno(int owner, int action, char *perms)
{
    // printf("perms %s\n", perms);
    int len = strlen(perms);
    char user_p = perms[len - 3] - '0';
    user_p = (int)user_p;
    char group_p = perms[len - 2] - '0';
    group_p = (int)group_p;
    char other_p = perms[len - 1] - '0';
    other_p = (int)other_p;
    int cmp = 0;
    int mat[3][4] =
        {{4, 5, 6, 7}, {2, 3, 6, 7}, {1, 3, 5, 7}};

    if (owner == 0)
        cmp = user_p;
    if (owner == 1)
        cmp = group_p;
    if (owner == 2)
        cmp = other_p;
    // printf("%d %d %d\n", owner, cmp, len);
    for (int i = 0; i < 4; i++)
    {
        if (mat[action][i] == cmp)
            return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        char str[100] = "Please pass filepath as arguement\n";
        write(0, str, strlen(str));
        return 0;
    }
    // read from input file
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        perror("program");
        return 0;
    }
    char *file = argv[1];
    char dir[10000] = "Assignment";
    char file_1[10000] = "Assignment/1_";
    char file_2[10000] = "Assignment/2_";
    strcat(file_1, file);
    strcat(file_2, file);

    // char file_1_name[100] = "1_";
    // char file_2_name[100] = "2_";
    // strcat(file_1_name, argv[1]);
    // strcat(file_2_name, argv[1]);

    struct stat sb;
    stat(file_1, &sb);
    if ((sb.st_mode & S_IFMT) == S_IFREG)
    {
        printf("Directory is created: Yes\n");
        unsigned code = sb.st_mode;
        char perms[10];
        sprintf(perms, "%o", code);
        sprintf(perms, "%s", perms);
        int perms_len = strlen(perms);
        char owner_arr[3][10] = {
            "User",
            "Group",
            "Others"};
        char action_arr[3][10] = {
            "read",
            "write",
            "execute"};

        for (int i = 0; i < 3; i++)
        {
            printf("\n");
            for (int j = 0; j < 3; j++)
            {
                int t = yesorno(i, j, &perms);
                char bool[3];
                if (t == 1)
                    printf("%s has %s permission on %s : %s\n", owner_arr + i, action_arr + j, file_1, "Yes");
                else
                    printf("%s has %s permission on %s : %s\n", owner_arr + i, action_arr + j, file_1, "No");
            }
        }
    }
    else
        perror("Error");

    printf("\n");
    struct stat sb2;
    stat(file_2, &sb2);
    if ((sb.st_mode & S_IFMT) == S_IFREG)
    {
        printf("Directory is created: Yes\n");
        unsigned code = sb2.st_mode;
        char perms[10];
        sprintf(perms, "%o", code);
        sprintf(perms, "%s", perms);
        int perms_len = strlen(perms);
        char owner_arr[3][10] = {
            "User",
            "Group",
            "Others"};
        char action_arr[3][10] = {
            "read",
            "write",
            "execute"};

        for (int i = 0; i < 3; i++)
        {
            printf("\n");
            for (int j = 0; j < 3; j++)
            {
                int t = yesorno(i, j, &perms);
                char bool[3];
                if (t == 1)
                    printf("%s has %s permission on %s : %s\n", owner_arr + i, action_arr + j, file_2, "Yes");
                else
                    printf("%s has %s permission on %s : %s\n", owner_arr + i, action_arr + j, file_2, "No");
            }
        }
    }
    else
        perror("Error");

    printf("\n");
    struct stat sb3;
    stat(dir, &sb3);
    if ((sb3.st_mode & S_IFMT) == S_IFDIR)
    {
        printf("Directory is created: Yes\n");
        unsigned code = sb3.st_mode;
        char perms[10];
        sprintf(perms, "%o", code);
        sprintf(perms, "%s", perms);
        int perms_len = strlen(perms);
        char owner_arr[3][10] = {
            "User",
            "Group",
            "Others"};
        char action_arr[3][10] = {
            "read",
            "write",
            "execute"};

        for (int i = 0; i < 3; i++)
        {
            printf("\n");
            for (int j = 0; j < 3; j++)
            {
                int t = yesorno(i, j, &perms);
                char bool[3];
                if (t == 1)
                    printf("%s has %s permission on %s : %s\n", owner_arr + i, action_arr + j, dir, "Yes");
                else
                    printf("%s has %s permission on %s : %s\n", owner_arr + i, action_arr + j, dir, "No");
            }
        }
    }
    else
        perror("Error");
}