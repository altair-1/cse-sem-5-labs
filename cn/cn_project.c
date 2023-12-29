//server code
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
" Q1 : ␣ What ␣ is ␣ the ␣ Capital ␣ of ␣ France ? " ,
" Q2 : ␣ When ␣ was ␣ the ␣ Constitution ␣ established ? " ,
" Q3 : ␣ What ␣ temperature ␣ does ␣ water ␣ boil ␣ at ? " ,
" Q4 : ␣ What ␣ temperature ␣ does ␣ water ␣ freeze ␣ at ? " ,
" Q5 : ␣ How ␣ many ␣ continents ␣ are ␣ there ? "
};

char answers [MAX_QUESTIONS][256] = {
" Paris " ,
" 1949 " ,
" 100 " ,
" 0 " ,
" 7 "
};

void error (const char * msg){
	perror (msg);
	exit (1);
}

void *handleClient (void* arg){
	int clisockfd =( int) arg ;
	int q =0 , score =0;
	char ans [256] , buf [256];
	for( q =0; q < MAX_QUESTIONS ; q ++) {
		write ( clisockfd , questions [ q ] ,256) ;
		memset ( ans ,0 ,256) ;
		read ( clisockfd , ans ,256) ;
		memset ( buf ,0 ,256) ;
		if( strcasecmp ( ans , answers [ q ]) ==0) {
			sprintf ( buf , " Correct !" ) ;
			score ++;
		}
		else
			sprintf ( buf , " Wrong .. ") ;
			write ( clisockfd , buf , sizeof( buf ) );
	}
	if (score == MAX_QUESTIONS) {
		sprintf (buf, "Congratulations ! ␣ You ␣ won ! " ) ;
	} 
	else{
		sprintf (buf, "You lost" ) ;
	}
	write (clisockfd,buf,sizeof(buf));
	printf (" %d ␣ scored ␣ %d ␣ out ␣ of ␣ 5\n" ,clisockfd,score);
}

int main () {
	int mainsockfd , clisockfd ;
	struct sockaddr_in servadd , cliadd ;
	pthread_t tid ;
	// creating the main socket that listens for connections
	mainsockfd = socket ( AF_INET , SOCK_STREAM ,0) ;
	if( mainsockfd <0) {
		perror ( " Socket ␣ not ␣ created \n");
		exit ( EXIT_FAILURE );
	}
	servadd . sin_family = AF_INET ;
	servadd . sin_addr . s_addr = INADDR_ANY ;
	servadd . sin_port = htons ( PORT ) ;
	int opt = 1;
	setsockopt (mainsockfd,SOL_SOCKET,SO_REUSEADDR,(void*)&opt,sizeof(opt));
	int res = bind (mainsockfd,(struct sockaddr *)&servadd,sizeof(servadd));
	if( res == -1) {
		perror ( " Bind ␣ error \n " ) ;
		exit ( EXIT_FAILURE );
	}
	printf("Server ␣ listening ␣ on ␣ port ␣ % d \n",PORT);
	listen ( mainsockfd , MAX_CLIENTS );
	int i =0;
	while (1){
		if(i > MAX_CLIENTS ) 
			break;
		int clilen = sizeof( cliadd ) ;
		clisockfd = accept(mainsockfd,(struct sockaddr *) &cliadd,&clilen);
		if( clisockfd <0) {
			perror("Accept ␣ error\n");
		}
		printf("Client ␣ % d ␣ entered . ␣ ID :%d\n",i+1,clisockfd);
		i++;
		pthread_create (& tid , NULL , ( void *) handleClient,(void *)clisockfd);
	}
}

//client code
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_ANSWER_LEN 256
#define MAX_QUESTIONS 5

int main () {
	int c_sock ;
	char buf [ MAX_ANSWER_LEN ];
	char msg [ MAX_ANSWER_LEN ] , val [ MAX_ANSWER_LEN ];
	c_sock = socket ( AF_INET , SOCK_STREAM , 0) ;
	struct sockaddr_in client ;
	memset(&client,0,sizeof(client));
	client.sin_family = AF_INET;
	client.sin_port = htons(8888); // Connect to the server on port 8888
	client . sin_addr . s_addr = inet_addr ( " 127.0.0.1 " ) ;
	if ( connect ( c_sock , ( struct sockaddr *) & client , sizeof(client ) ) == -1) {
		printf ( " Error : ␣ Server ␣ seems ␣ to ␣ be ␣ down ␣ or ␣ game ␣ member ␣ limit ␣ reached . ␣ Exiting ...\n");
		exit (0) ;
	}
	printf ( " Establishing ␣ connection ␣ to ␣ the ␣ game ␣ server ...\n\n");
	int i = 0;
	while (i < MAX_QUESTIONS ) {
		msg [0]=0 , buf [0]=0 , val [0]=0;
		int bytes_received = read ( c_sock , msg ,MAX_ANSWER_LEN ) ;
		if ( bytes_received <= 0) {
			close(c_sock);
		}
		printf("%s\n",msg);
		scanf ( " % s " , buf ) ;
		write ( c_sock , buf , sizeof( buf ) );
		bytes_received = read ( c_sock , val , MAX_ANSWER_LEN ) ;
		if ( bytes_received <= 0) {
			close ( c_sock );
		}
		printf ( "% s \n " , val ) ;
		i ++;
	}
	int val_received = read ( c_sock , msg , MAX_ANSWER_LEN );
	if ( val_received <= 0) {
		close ( c_sock );
	}
	printf ( " %s \n " , msg ) ;
	close ( c_sock );
	return 0;
}