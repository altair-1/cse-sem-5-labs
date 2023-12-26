#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#define PORTNO 8003

int main(){
    int sd;
    struct sockaddr_in address;
    sd = socket(AF_INET,SOCK_DGRAM,0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("172.16.48.84");
    address.sin_port = htons(PORTNO);
    int m,n;
    int len = sizeof(address);
    printf("enter number of rows: ");
    scanf("%d",&m);
    sendto(sd,&m,sizeof(m),0,(struct sockaddr *)&address, len);
    printf("enter number of columns: ");
    scanf("%d",&n);
    sendto(sd,&n,sizeof(n),0,(struct sockaddr *)&address, len);
    int arr[n];
    int rowCount = 0;
    while(rowCount < m){
        for(int i=0;i<n;i++){
            scanf("%d",&arr[i]);
        }
        sendto(sd,arr,n*sizeof(int),0,(struct sockaddr *)&address, len);
        rowCount++;
    }
    return 0;
}