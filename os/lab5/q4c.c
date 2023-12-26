#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#define MAX 5
#define FIFO "q4_fifo"

void main(){
    int shelf;
    int pipe_fd = open(FIFO, O_RDONLY);
    if(pipe_fd<0){
        perror("error\n");
        exit(1);
    }
    printf("consumer starts\n");
    while(1){
        read(pipe_fd,&shelf,sizeof(int));
        printf("added to shelf\nsize: %d\n",shelf);
        if(shelf>0){
            shelf--;
            printf("consumer consumed\nshelf: %d\n",shelf);
            sleep(3);
            write(pipe_fd,&shelf,sizeof(int));
        }
        else
            printf("\nwaiting for producer");
    }
    close(pipe_fd);
}