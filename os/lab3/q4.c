#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void main(){
    pid_t pid;
    pid=fork();
    if(pid==-1){
        printf("error\n");
    }
    else if(pid==0){
        exit(0);
    }
    else{
        sleep(100);
        printf("program 2 parent process\n");
    }
}