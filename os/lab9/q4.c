#include <stdio.h>

#define MAX_FRAMES 3

typedef struct{
    int pageNum;
    int referenced;
} Page;

int main(){
    int reference_string[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    int reference_string_size = sizeof(reference_string) / sizeof(reference_string[0]);
    Page frames[MAX_FRAMES];
    int totalPF = 0;
    int hits = 0;
    for (int i = 0; i < MAX_FRAMES; i++) {
        frames[i].pageNum = -1; 
        frames[i].referenced = 0;
    }
    for (int i = 0; i < reference_string_size; i++){
        int pageNum = reference_string[i];
        int page_found = 0;
        for (int j = 0; j < MAX_FRAMES; j++){
            if (frames[j].pageNum == pageNum) {
                frames[j].referenced = 1;
                hits++;
                page_found = 1;
                break;
            }
        }
        if (!page_found) {
            int victim_found = 0;
            while (!victim_found) {
                for (int j = 0; j < MAX_FRAMES; j++){
                    if (frames[j].referenced == 0){
                        frames[j].pageNum = pageNum;
                        frames[j].referenced = 0;
                        victim_found = 1;
                        break;
                    } 
                    else{
                        frames[j].referenced = 0; 
                    }
                }
            }
            totalPF++;
        }
        printf("current frames: ");
        for (int j = 0; j < MAX_FRAMES; j++){
            if (frames[j].pageNum == -1){
                printf("[ ] ");
            } 
            else{
                printf("[%d] ", frames[j].pageNum);
            }
        }
        printf("\n");
    }
    printf("total page faults: %d\n", totalPF);
    printf("hit ratio: %.2f\n", (float)hits / reference_string_size);
    return 0;
}