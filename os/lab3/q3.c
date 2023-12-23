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
        printf("child process id from child: %d\n",getpid());
        printf("parent process id from child: %d\n",getppid());
    }
    else{
        printf("child process id from parent: %d\n",pid);
        printf("parent process id from parent: %d\n",getpid());
    }
}