// C program to illustrate 
// read system Call 
#include<stdio.h> 
#include<fcntl.h> 
#include<stdlib.h>
  
int main() 
{ 
    char BUFFER; 
    int fd1 = open("foobar.txt", O_RDONLY, 0); 
    read(fd1, &BUFFER, 1); 
    printf("BUFFER = %c\n", BUFFER); 
    exit(0); 
} 