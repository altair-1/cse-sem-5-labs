#include <stdio.h>
#include <stdlib.h> // For exit()

int main(){
	FILE *fptr1, *fptr2;
	char filename[100], c;
	printf("enter the filename to open for reading: ");
	scanf("%s", filename);
	fptr1 = fopen(filename, "r");
	// open one file for reading
	if (fptr1 == NULL){
		printf("cannot open file %s \n", filename);
		exit(0);
	}

	printf("enter the filename to open for writing: ");
	scanf("%s", filename);
	fptr2 = fopen(filename, "w+"); // open another file for writing
	c = fgetc(fptr1);
	// read contents from file
	while (c != EOF){
		fputc(c, fptr2);
		c = fgetc(fptr1);
	}
	printf("\ncontents copied to %s \n", filename);
	fclose(fptr1);
	fclose(fptr2);
	return 0;
}