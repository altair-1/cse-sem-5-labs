#include <stdio.h>

//print array
void printArray(int array [], float size){
	for (int i = 0; i <= size; ++i){
		printf("%d", array[i]);
	}
	printf("\n");
}

int main(){
	int data[] = {-2,45,0,11,-9};

	//find array's length
	int size = sizeof(data)/sizeof(data[0]);

	printf("entered array:	\n");
	printArray(data, size);
}