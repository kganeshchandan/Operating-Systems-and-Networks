// C program to illustrate 
// read system Call 
#include<stdio.h> 
#include<stdlib.h>
#include<fcntl.h> 
int main() 
{ 
  int fd, sz; 

  // declaring a buffer
  char *BUFFER = (char *) calloc(100, sizeof(char)); 
  
  fd = open("foo.txt", O_RDONLY); 
  if (fd < 0) { perror("r1"); exit(1); } 
  
  sz = read(fd, BUFFER, 10); 
  printf("called read(%d, c, 10).  returned that"
         " %d bytes  were read.\n", fd, sz); 
  BUFFER[sz] = '\0'; 
  printf("Those bytes are as follows: %s\n", BUFFER); 
} 