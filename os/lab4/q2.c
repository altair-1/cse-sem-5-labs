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
    printf("device id containing the file: %ld\t\n",sbuf.st_dev);
    printf("inode number: %ld\t\n",sbuf.st_ino);
    printf("permissions : %d\t\n",sbuf.st_mode);
    printf("no. of hard links : %ld\t\n",sbuf.st_nlink);
    printf("owner user id: %d\t\n",sbuf.st_uid);
    printf("group user id: %d\t\n",sbuf.st_gid);
    printf("device id: %ld\t\n",sbuf.st_rdev);
    printf("total size in bytes: %ld\t\n",sbuf.st_size);
    printf("block size: %ld\t\n",sbuf.st_blksize);
    printf("no. of blocks allocated: %ld\t\n",sbuf.st_blocks);
    printf("last access time: %ld\t\n",sbuf.st_atime);
    printf("last modification time: %ld\t\n",sbuf.st_mtime);
    printf("last status change time: %ld\t\n",sbuf.st_ctime);
}