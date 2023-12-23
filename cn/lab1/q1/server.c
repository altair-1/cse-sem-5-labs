#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#define PORTNO 10200

int main() {
	int sockfd, newsockfd, portno, clilen, n = 1;
	struct sockaddr_in seraddr, cliaddr;
	int i, value;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	seraddr.sin_family = AF_INET;
	seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	seraddr.sin_port = htons(PORTNO);
	bind(sockfd, (struct socaddr *)&seraddr, sizeof(seraddr));
	listen(sockfd, 5);
	while (1) {
		char buf[256];
		printf("server waiting");
		// accept a connection
		clilen = sizeof(clilen);
		newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
		n = read(newsockfd, buf, sizeof(buf));
		printf(" \nencrypted message from client: %s \n", buf);
        for(i = 0; i < strlen(buf); i++) 
            buf[i] = buf[i] - 4;
        printf("decrypted message from client: %s \n", buf);
		n = write(newsockfd, buf, sizeof(buf));
	}
}