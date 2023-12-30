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
	int c_sock;
	char buf [MAX_ANSWER_LEN];
	char msg [MAX_ANSWER_LEN], val [MAX_ANSWER_LEN];
	c_sock = socket (AF_INET,SOCK_STREAM,0);
	struct sockaddr_in client;
	memset(&client,0,sizeof(client));
	client.sin_family = AF_INET;
	client.sin_port = htons(8888); //connect to the server on port 8888
	client.sin_addr.s_addr = inet_addr ("127.0.0.1");
	if (connect(c_sock, (struct sockaddr *)&client, sizeof(client)) == -1){
		printf ("error! server seems to be down or game member limit reached\nexiting\n");
		exit (0) ;
	}
	printf ("establishing connection to the game server\n");
	int i = 0;
	while (i < MAX_QUESTIONS){
		msg[0]=0, buf[0]=0, val[0]=0;
		int bytes_received = read (c_sock,msg,MAX_ANSWER_LEN);
		if (bytes_received <= 0){
			close(c_sock);
		}
		printf("%s\n",msg);
		scanf ("%s",buf);
		write (c_sock,buf,sizeof(buf));
		bytes_received = read (c_sock,val,MAX_ANSWER_LEN);
		if (bytes_received <= 0) {
			close (c_sock);
		}
		printf("%s\n",val);
		i++;
	}
	int val_received = read (c_sock,msg,MAX_ANSWER_LEN);
	if (val_received <= 0){
		close(c_sock);
	}
	printf("%s\n",msg);
	close(c_sock);
	return 0;
}