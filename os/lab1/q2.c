#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h> 

void main(int argc,char* argv[]){
	if(argc != 2){
		printf("insufficient arguments\n");
		exit(1);
	}
	int fd=open(argv[1],O_RDONLY);
	if(fd==-1){
		printf("file not found\n");
		exit(1);
	}
	char c,temp[100];
	int i=0,k=0;
	while(read(fd,&c,1)>0){
		if(c!='\n'){
			temp[i]=c;
			i++;
		}
		else{
			k++;
			temp[i]='\0';
			i=0;
			printf("line %d : %s \n", k,temp);
			if(k%20==0){
				char key;
				printf("\npress any key to continue : \t");
                scanf("%c",&key);
				printf("\n");
            }
		}
	}
	exit(0);
}