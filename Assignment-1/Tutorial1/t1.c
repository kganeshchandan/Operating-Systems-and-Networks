// C program to illustrate 
// open system call 
#include<stdio.h> 
#include<fcntl.h> 
#include<errno.h> 
extern int errno; 
int main() 
{      
    // if file does not have in directory  
    // then file foo.txt is created. 
    int fd = open("foo1.txt", O_RDONLY | O_CREAT, 0644);  
      
    // printing the file descriptor
    printf("fd = %d\n", fd); 

    // error handling 
    if (fd ==-1) 
    { 
        // print which type of error have in a code 
        printf("Error Number % d\n", errno); 
          
        // print program detail "Success or failure" 
        perror("Program");                  
    } 
    return 0; 
} 