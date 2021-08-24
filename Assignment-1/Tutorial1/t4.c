// C program to illustrate 
// read system Call 
#include<stdio.h> 
#include<fcntl.h> 
#include<stdlib.h>
#include<string.h>
int main() 
{ 
    char BUFFER[20]; 
    int fd1 = open("foobar.txt", O_RDONLY); 
    read(fd1, &BUFFER, 10); 
    BUFFER[10]='\0';
    printf("BUFFER = %s\n", BUFFER); 
    exit(0); 
} 