#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

#define PORT 8888
#define MAX_CLIENTS 5
#define MAX_QUESTIONS 5
#define MAX_ANSWER_LEN 256

char questions [MAX_QUESTIONS][256] = {
"q1: what is the capital of russia?",
"q2: when did india gain independence?",
"q3: what is the largest planet of our solar system?",
"q4: how many hours are there in a day?",
"q5: how many colours are there in a rainbow?"
};

char answers [MAX_QUESTIONS][256] = {
"moscow",
"1947",
"jupiter",
"24",
"7"
};

void error (const char * msg){
	perror (msg);
	exit (1);
}

void *handleClient (void* arg){
	int clisockfd = (int)arg;
	int q = 0,score = 0;
	char ans [256], buf [256];
	for( q = 0; q < MAX_QUESTIONS; q ++){
		write (clisockfd, questions [q],256);
		memset (ans,0,256);
		read (clisockfd,ans,256);
		memset (buf,0,256);
		if (strcasecmp (ans,answers[q]) == 0){
			sprintf(buf,"correct!");
			score ++;
		}
		else
			sprintf (buf,"wrong");
			write (clisockfd,buf,sizeof(buf));
	}
	if (score == MAX_QUESTIONS){
		sprintf (buf, "congratulations! you won");
	} 
	else{
		sprintf (buf,"you lost");
	}
	write (clisockfd,buf,sizeof(buf));
	printf ("id %d scored %d out of 5\n",clisockfd,score);
}

int main (){
	int mainsockfd,clisockfd;
	struct sockaddr_in servadd,cliadd;
	pthread_t tid ;
	// creating the main socket that listens for connections
	mainsockfd = socket (AF_INET,SOCK_STREAM,0);
	if( mainsockfd <0) {
		perror ("socket not created\n");
		exit (EXIT_FAILURE);
	}
	servadd.sin_family = AF_INET;
	servadd.sin_addr.s_addr = INADDR_ANY;
	servadd.sin_port = htons(PORT);
	int opt = 1;
	setsockopt (mainsockfd,SOL_SOCKET,SO_REUSEADDR,(void *)&opt, sizeof(opt));
	int res = bind (mainsockfd,(struct sockaddr *)&servadd, sizeof(servadd));
	if( res == -1){
		perror ("bind error\n");
		exit (EXIT_FAILURE);
	}
	printf("server listening on port %d\n",PORT);
	listen (mainsockfd,MAX_CLIENTS);
	int i = 0;
	while (1){
		if(i > MAX_CLIENTS) 
			break;
		int clilen = sizeof(cliadd);
		clisockfd = accept(mainsockfd,(struct sockaddr *) &cliadd,&clilen);
		if(clisockfd < 0){
			perror("accept error\n");
		}
		printf("client %d entered id %d\n",i+1,clisockfd);
		i++;
		pthread_create(&tid,NULL,(void *)handleClient,(void *)clisockfd);
	}
}