// C program to illustrate 
// write system Call 
#include<stdio.h> 
#include <fcntl.h> 
main() 
{ 
  int sz; 
  
  // opening a file to write
  int fd = open("foo.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644); 

  // error handling
  if (fd < 0) 
  { 
     perror("r1"); 
     exit(1); 
  } 
  
  // writing to the file
  sz = write(1, "hello world1234\n", strlen("hello world1234\n")); 
  
  printf("called write(% d, \"hello world\\n\", %d)."
    " It returned %d\n", fd, strlen("hello world\n"), sz); 
  
  close(fd); 
} 