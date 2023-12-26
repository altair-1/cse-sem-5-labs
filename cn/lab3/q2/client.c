#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#define SERVER_IP "172.217.168.46"
#define PORT 80

void main() {
    struct sockaddr_in address;
    char response[4096];
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(SERVER_IP);
    address.sin_port = htons(PORT);
    if (connect(clientfd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("client connection error\n");
        exit(1);
    }
    char request[] = "GET / HTTP/1.1\r\nHost: www.google.com\r\n\r\n | nc www.google.com 80";
    send(clientfd, request, strlen(request), 0);
    recv(clientfd, response, sizeof(response), 0);
    printf("received HTTP response:\n%s\n", response);
    close(clientfd);
}