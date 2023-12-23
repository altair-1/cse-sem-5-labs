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
	int fd=open(argv[2],O_RDONLY);
	if(fd==-1){
		printf("file not found\n");
		exit(1);
	}
	char c,temp[100];
	int i=0,k=0;
	while(read(fd,&c,1)>0){
		if(c!='\n'){
			temp[i++]=c;
		}
		else{
			k++;
			temp[i]='\0';
			i=0;
			if(strstr(temp,argv[1])){
                printf("line %d : %s \n", k,temp);
            }
		}
	}
	exit(0);
}