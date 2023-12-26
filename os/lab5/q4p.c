#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#define MAX 5
#define FIFO "q4_fifo"

void main(){
    int shelf=0;
    if (access(FIFO, F_OK) == -1){
        int res = mkfifo(FIFO, 0777);
        if (res != 0){
            perror("could not create\n");
            exit(1);
        }
    }
    int pipe_fd = open(FIFO, O_WRONLY);
    if(pipe_fd<0){
        perror("error");
        exit(1);
    }
    printf("producer starts\n");
    while(1){
        if(shelf<MAX){
            shelf++;
            write(pipe_fd,&shelf,sizeof(int));
            printf("producer added\nshelf: %d\n",shelf);
        }
        sleep(1);
        read(pipe_fd,&shelf,sizeof(int));
    }
    close(pipe_fd);
}