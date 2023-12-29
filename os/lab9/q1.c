#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int algo;

typedef struct mab Mab;
typedef Mab* MabPtr;

struct mab{
    int offset;
    int size;
    int allocated;
    MabPtr next;
    MabPtr prev;
};

MabPtr memChk(MabPtr m,int size){
    MabPtr temp=m;
    MabPtr ret = NULL;
    while(temp!=NULL){
        if(algo == 1){
            if(temp->size - temp->allocated >= size){
                if(ret==NULL){
                    ret = temp;
                }
                else{
                    if(ret->size - ret->allocated >= temp->size - temp->allocated){
                        ret = temp;
                    }
                }
            }
        }
        else if(algo == 2){
            if(temp->size - temp->allocated >= size){
                ret = temp;
                break;
            }
        }
        else if(algo == 3){
            if(temp->size - temp->allocated >= size){
                if(ret==NULL){
                    ret = temp;
                }
                else{
                    if(ret->size - ret->allocated <= temp->size - temp->allocated){
                        ret = temp;
                    }
                }
            }
        }
        else{
            printf("incorrect choice\n");
            exit(EXIT_FAILURE);
        }
        temp = temp->next;
    }
    return ret;
}

MabPtr getNode(){
    return (MabPtr)malloc(sizeof(Mab));
}

MabPtr takeInput(){
    int n;
    printf("enter no. of blocks: ");
    scanf("%d",&n);
    MabPtr head = NULL;
    int off=0;
    int size;
    int allocated;
    MabPtr prev=NULL;
    for(int i=0;i<n;i++){
        printf("block %d\n",i+1);
        printf("enter size: ");
        scanf("%d",&size);
        printf("enter allocated size: ");
        scanf("%d",&allocated);
        if(allocated > size){
            printf("not possible\n");
            i--;
            continue;
        }
        head=getNode();
        head->offset = off;
        head->size = size;
        head->allocated = allocated;
        head->next = NULL;
        head->prev = prev;
        off += head->size;
        if(prev != NULL){
            prev->next=head;
        }
        prev = head;
        head = head->next;
    }
    while(prev->prev){
        prev = prev->prev;
    }
    return prev;
}

MabPtr memAlloc(MabPtr m,int size){
    m->allocated += size;
    return m;
}

MabPtr memFree(MabPtr m){
    m->allocated = 0;
    return m;
}

void DisplayMemoryBlocks(MabPtr m){
    MabPtr temp = m;
    if(!temp){
        printf("no memory blocks\n");
        return;
    }
    int i=1;
    printf("display\n");
    while(temp){
        printf("block %d\n",i);
        printf("offset: %d\n",temp->offset);
        printf("size:%d\n",temp->size);
        printf("allocated: %d\n",temp->allocated);
        i++;
        temp = temp->next;
    }
}

void main(){
    MabPtr head=takeInput();
    DisplayMemoryBlocks(head);
    printf("algorithms\n");
    printf("1) best fit\n2) first fit\n3) worst fit\n");
    printf("enter alogorithm to implement: ");
    scanf("%d",&algo);
    printf("enter size to check and allocate: ");
    int size;
    scanf("%d",&size);
    MabPtr ptr = memChk(head,size);
    ptr = memAlloc(ptr,size);
    DisplayMemoryBlocks(head);
    ptr = memFree(ptr);
    DisplayMemoryBlocks(head);
}