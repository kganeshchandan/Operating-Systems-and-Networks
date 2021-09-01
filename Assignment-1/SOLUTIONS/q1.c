// Reverse a large file
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

// extern int errno;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        char str[100] = "Please enter filepath as the arguement\n";
        write(0, str, strlen(str));
        return 0;
    }
    // read from input file
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        perror("Unable to open file");
        return 0;
    }
    // Create Assignment directory with read, write execute permissions
    mkdir("Assignment", 0700);

    // Extracting file name from path
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
    // return 0;
    char output_file_name[100] = "Assignment/1_";
    strcat(output_file_name, input_file_name);

    int fd2 = open(output_file_name, O_CREAT | O_RDWR | O_TRUNC, 0600);
    if (fd2 < 0)
    {
        perror("Unable to open file");
        return 0;
    }
    // getting the len of the file
    off_t input_file_len = lseek(fd, 0, SEEK_END);
    off_t to_read = input_file_len;

    long long int chunk_size = input_file_len;
    if (input_file_len > 1e6)
        chunk_size = 1e6;
    char *BUFFER = (char *)malloc(chunk_size);
    char *REVERSED_BUFFER = (char *)malloc(chunk_size);
    float percentage = 0.0;
    char buff[100];

    // pointing to start in output
    lseek(fd2, 0, SEEK_SET);
    while (to_read > 0)
    {
        percentage = (1.0 - (float)to_read / (float)input_file_len) * 100;

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
            to_read = 0;
            break;
        }
    }
    sprintf(buff, "\rProgress: %.2f\n", 100.00);
    write(0, buff, strlen(buff));
    fflush(stdout);
    close(fd);
    close(fd2);
    return 0;
}
