#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h> 

void main(int argc,char* argv[]){
	int a=7676;
    float b=9.06;
    char c='N';
    char str[]="hello";
  	printf("a=%d b=%f c=%c str=%s hexadecimal for a=%x \n",a,b,c,str,a);
}