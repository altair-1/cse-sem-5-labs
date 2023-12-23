#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h> 

void main(int argc,char* argv[]){
  if(argc != 3){
    printf("insufficient arguments\n");
    exit(1);
  }
  int fd=open(argv[1],O_RDONLY);
  int fl=open(argv[2],O_CREAT|O_WRONLY,S_IRUSR|S_IWUSR);
  if(fd==-1){
    printf("file not found\n");
    exit(1);
  }
  char c;
  while(read(fd,&c,1)==1){
    write(fl,&c,1);
  }
}