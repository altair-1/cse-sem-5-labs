#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

sem_t room;
sem_t ch[5];

void * philosopher(void *);
void eat(int);

int main(){
	int i,a[5];
	pthread_t tid[5];
	sem_init(&room,0,4);
	for(i=0;i<5;i++)
		sem_init(&ch[i],0,1);
	for(i=0;i<5;i++){
		a[i]=i;
		pthread_create(&tid[i],NULL,philosopher,(void *)&a[i]);
	}
	for(i=0;i<5;i++)
		pthread_join(tid[i],NULL);
}

void * philosopher(void * num){
	int phil=*(int *)num;
	sem_wait(&room);
	printf("\nphilosopher %d has entered room",phil);
	sem_wait(&ch[phil]);
	sem_wait(&ch[(phil+1)%5]);
	eat(phil);
	sleep(2);
	printf("\nphilosopher %d has finished eating\n",phil);
	sem_post(&ch[(phil+1)%5]);
	sem_post(&ch[phil]);
	sem_post(&room);
}

void eat(int phil){
	printf("\nphilosopher %d is eating",phil);
}