#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void main(){
    int status;
    pid_t pid;
    pid=fork();
    if(pid==-1){
        printf("error\n");
    }
    else if(pid==0){
        printf("child process\n");
        exit(0);
    }
    else{
        wait(&status);
        printf("parent process\n");
        printf("child process returned: %d\n",status);
    }
}