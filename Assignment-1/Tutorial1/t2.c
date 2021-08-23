// C program to illustrate close system Call 
#include<stdio.h> 
#include <fcntl.h> 
int main() 
{ 
    // opening the file
    int fd1 = open("foo.txt", O_RDONLY); 

    // error handling
    if (fd1 < 0)  
    { 
        perror("c1"); 
        exit(1); 
    } 

    // printing the file descriptor
    printf("opened the fd = % d\n", fd1); 
      
    // Using close system Call to close the file
    if (close(fd1) < 0)  
    { 
        perror("c1"); 
        exit(1); 
    }  
    printf("closed the fd.\n"); 
} 