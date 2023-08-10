#include <stdio.h>

void main() {
	FILE * fptr;
	char filename[100];
	printf("enter filename to open for reading: ");
	scanf("%s", filename);
	fptr = fopen(filename, "r");
	if (fptr != NULL) {
		int lineCount = 0, charCount = 0;
		char ch = fgetc(fptr);
		while(ch != EOF) {
			if (ch == '\n')
				lineCount++;
			else
				charCount++;
			ch = fgetc(fptr);
		}
		fclose(fptr);
		printf("number of lines: %d\nnumber of characters: %d\n", lineCount+1, charCount);
	}
	else 
		printf("could not open file %s\n", filename);
}