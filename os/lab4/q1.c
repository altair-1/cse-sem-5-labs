#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

void main(int argc,char* argv[100]){
    if(argc!=2){
        printf("ERROR! insufficient arguments\n");
        return;
    }
    struct stat sbuf;
    int ret=stat(argv[1],&sbuf);
    if(ret==-1){
        perror("ERROR\n");
        return;
    }
    printf("inode number: %ld\t\n",sbuf.st_ino);
}