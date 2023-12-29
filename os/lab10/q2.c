#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t lock;

int res, pro, ind = 0;
int* finish;
int* answer;
int* claim;
int* avail;
int** alloc;
int** max;
int** need;

void printMat(int** arr, int r, int c){
    for (int i = 0; i < r; i++){
        for (int j = 0; j < c; j++){
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}

void printArr(int* arr, int r){
    for (int i = 0; i < r; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void* Banker(void* param){
    int pid = *((int*)param);
    int flag = 0;
    pthread_mutex_lock(&lock);
    if(finish[pid]==1){
        return NULL;
    }
    printf("process %d requesting\n", pid);
    printf("process %d waiting\n", pid);
    for (int l = 0; l < res; l++){
        if (need[pid][l] > avail[l]){
            flag = 1;
            break;
        }
    }
    if (flag == 0){
        answer[ind++] = pid;
        for (int k = 0; k < res; k++){
            avail[k] += alloc[pid][k];
        }
        finish[pid] = 1;
    }
    printf("process %d terminated\n", pid);
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(){
    printf("enter no. of processes: ");
    scanf("%d", &pro);
    printf("enter no. of resources: ");
    scanf("%d", &res);
    claim = (int*)calloc(res, sizeof(int));
    printf("enter claim vector: \n");
    for (int i = 0; i < res; i++){
        scanf("%d", &claim[i]);
    }
    alloc = (int**)calloc(pro, sizeof(int*));
    printf("enter allocation matrix: \n");
    for (int i = 0; i < pro; i++){
        alloc[i] = (int*)calloc(res, sizeof(int));
        printf("enter allocation data for process %d: \n", i + 1);
        for (int j = 0; j < res; j++){
            scanf("%d", &alloc[i][j]);
        }
    }
    max = (int**)calloc(pro, sizeof(int*));
    need = (int**)calloc(pro, sizeof(int*));
    printf("enter max. matrix: \n");
    for (int i = 0; i < pro; i++){
        max[i] = (int*)calloc(res, sizeof(int));
        need[i] = (int*)calloc(res, sizeof(int));
        printf("enter max. data for process %d: \n", i + 1);
        for (int j = 0; j < res; j++){
            scanf("%d", &max[i][j]);
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }
    avail = (int*)calloc(res, sizeof(int));
    for (int i = 0; i < res; i++){
        avail[i] = 0;
        for (int j = 0; j < pro; j++){
            avail[i] += alloc[j][i];
        }
        avail[i] = claim[i] - avail[i];
        if (avail[i] < 0) {
            perror("incorrect data entered\n");
            exit(EXIT_FAILURE);
        }
    }
    finish = (int*)calloc(pro, sizeof(int));
    answer = (int*)calloc(pro, sizeof(int));
    printf("claim vector\n");
    printArr(claim, res);
    printf("allocation\n");
    printMat(alloc, pro, res);
    printf("max\n");
    printMat(max, pro, res);
    printf("need\n");
    printMat(need, pro, res);
    printf("available vector\n");
    printArr(avail, res);
    for (int i = 0; i < pro; i++){
        finish[i] = 0;
    }
    pthread_mutex_init(&lock, NULL);
    for(int m=0;m<pro;m++){
        pthread_t th[pro];
        for (int i = 0; i < pro; i++){
            int* process_id = (int*)malloc(sizeof(int));
            *process_id = i;
            pthread_create(&th[i], NULL, Banker, process_id);
        }
        for (int i = 0; i < pro; i++){
            pthread_join(th[i], NULL);
        }
    }
    pthread_mutex_destroy(&lock);
    int flag = 1;
    for (int i = 0; i < pro; i++){
        if (finish[i] == 0){
            flag = 0;
            printf("following system is unsafe\n");
            break;
        }
    }
    if (flag == 1){
        printf("safe sequence for the given system\n");
        for (int i = 0; i < pro - 1; i++){
            printf(" P%d -> ", answer[i]);
        }
        printf(" P%d\n", answer[pro - 1]);
    }
    return 0;
}