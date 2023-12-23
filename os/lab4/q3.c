#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

void main(int argc,char* argv[100]){
    if(argc!=3){
        printf("ERROR! insufficient arguments\n");
        return;
    }
    struct stat sbuf;
    int ret=stat(argv[1],&sbuf);
    if(ret==-1){
        perror("ERROR\n");
        return;
    }
    int flag=link(argv[1],argv[2]);
    if(flag==-1){
        perror("link error\n");
        return;
    }
    printf("successfully linked %s to %s\n",argv[1],argv[2]);
    flag=unlink(argv[2]);
    if(flag==-1){
        perror("unlink error\n");
        return;
    }
    printf("successfully unlinked %s to %s\n",argv[1],argv[2]);
}