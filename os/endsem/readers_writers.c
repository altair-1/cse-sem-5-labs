#include<semaphore.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

sem_t x,y;
pthread_t tid;
pthread_t writerthreads[100],readerthreads[100];
int readercount = 0;

void *reader(void* param){
    sem_wait(&x);
    readercount++;
    if(readercount==1)
    sem_wait(&y);
    sem_post(&x);
    printf("\nreader %d is inside",readercount);
    usleep(3);
    sem_wait(&x);
    readercount--;
    if(readercount==0){
        sem_post(&y);
    }
    sem_post(&x);
    printf("\nreader %d is leaving",readercount+1);
}

void *writer(void* param){
    printf("\nwriter is trying to enter");
    sem_wait(&y);
    printf("\nwriter has entered");
    sem_post(&y);
    printf("\nwriter is leaving\n");
}

int main(){
    int n2,i;
    printf("enter number of readers: ");
    scanf("%d",&n2);
    int n1[n2];
    sem_init(&x,0,1);
    sem_init(&y,0,1);
    for(i=0;i<n2;i++){
        pthread_create(&writerthreads[i],NULL,reader,NULL);
        pthread_create(&readerthreads[i],NULL,writer,NULL);
    }
    for(i=0;i<n2;i++){
        pthread_join(writerthreads[i],NULL);
        pthread_join(readerthreads[i],NULL);
    }
}