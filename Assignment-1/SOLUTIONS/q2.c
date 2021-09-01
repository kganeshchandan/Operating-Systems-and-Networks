// Reverse a large file
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        char str[150] = "Please enter filepath , no of parts, part to be reversed as the only arguements\n";
        write(0, str, strlen(str));
        return 0;
    }
    // read from input file
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        perror(argv[1]);
        return 0;
    }
    // Create Assignment directory with read, write execute permissions
    mkdir("Assignment", 0700);

    char input_file_name[strlen(argv[1])];
    long long int k = 0;
    for (long long int i = 0; i < strlen(argv[1]); i++)
    {
        input_file_name[k] = argv[1][i];
        if (argv[1][i] == '/')
        {
            k = 0;
        }
        else
        {
            k = k + 1;
        }
    }
    input_file_name[k] = '\0';

    char no_of_parts = argv[2][0] - '0';
    char part_to_be_reversed = argv[3][0] - '0';

    if (no_of_parts < part_to_be_reversed)
    {
        char str2[150] = "Error: no of parts must not be < part to be reversed\n";
        write(0, str2, strlen(str2));
        return 0;
    }
    no_of_parts = (long long int)no_of_parts;
    part_to_be_reversed = (long long int)part_to_be_reversed;
    // strcat(input_file_name, argv[1]);

    char output_file_name[100] = "Assignment/2_";
    strcat(output_file_name, input_file_name);

    int fd2 = open(output_file_name, O_CREAT | O_RDWR | O_TRUNC, 0600);
    if (fd2 < 0)
    {
        perror("Program");
        return 0;
    }
    // getting the len of the file
    off_t input_file_len = lseek(fd, 0, SEEK_END);
    // off_t to_read = input_file_len;

    long long int len_to_rev = input_file_len / (long long int)no_of_parts;
    long long int to_read = len_to_rev;
    long long int chunk_size = len_to_rev;
    if (len_to_rev > 1e6)
        chunk_size = 1e6;

    char *BUFFER = (char *)malloc(chunk_size);
    char *REVERSED_BUFFER = (char *)malloc(chunk_size);
    float percentage = 0.0;
    char buff[100];

    lseek(fd, part_to_be_reversed * len_to_rev, SEEK_SET);

    // pointing to start in output
    lseek(fd2, 0, SEEK_SET);
    while (to_read > 0)
    {
        percentage = (1.0 - (float)to_read / (float)len_to_rev) * 100;
        sprintf(buff, "\rProgress: %.2f", percentage);
        write(0, buff, strlen(buff));
        fflush(stdout);
        if (to_read >= chunk_size)
        {
            to_read = to_read - chunk_size;
            lseek(fd, -chunk_size, SEEK_CUR);
            read(fd, BUFFER, chunk_size);
            lseek(fd, -chunk_size, SEEK_CUR);
            for (long long int i = 0, j = chunk_size - 1; i < chunk_size; i++, j--)
            {
                REVERSED_BUFFER[i] = BUFFER[j];
            }
            BUFFER[chunk_size] = '\0';
            REVERSED_BUFFER[chunk_size] = '\0';
            write(fd2, REVERSED_BUFFER, chunk_size);
        }
        else if (to_read<chunk_size & to_read> 0)
        {
            chunk_size = to_read;
            lseek(fd, -chunk_size, SEEK_CUR);
            read(fd, BUFFER, chunk_size);
            lseek(fd, -chunk_size, SEEK_CUR);
            for (long long int i = 0, j = chunk_size - 1; i < chunk_size; i++, j--)
            {
                REVERSED_BUFFER[i] = BUFFER[j];
            }
            BUFFER[chunk_size] = '\0';
            REVERSED_BUFFER[chunk_size] = '\0';
            write(fd2, REVERSED_BUFFER, chunk_size);
            // to_read = 0;
            break;
        }
    }
    sprintf(buff, "\rProgress: %.2f\n", 100.00);
    write(0, buff, strlen(buff));
    fflush(stdout);
    close(fd);
    close(fd2);
    return 0;
    // pointing to end in o
}