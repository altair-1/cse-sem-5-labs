#include <stdio.h>

void main() {
	FILE * fptr1, * fptr2;
	char filename1[100], filename2[100];
	printf("enter filename to open for reading: ");
	scanf("%s", filename1);
	printf("enter filename to open for writing: ");
	scanf("%s", filename2);
	fptr1 = fopen(filename1, "r");
	if (fptr1 != NULL) {
		fptr2 = fopen(filename2, "w+");
		char ch;
		int i = -1;
		while (ftell(fptr1) != 1) {
			fseek(fptr1, i--, SEEK_END);
			ch = fgetc(fptr1);
			fputc(ch, fptr2);
		}
		printf("%s contains reverse of %s\n", filename2, filename1);
		fseek(fptr1, 0, SEEK_END);
		printf("size of %s: %ld bytes\n", filename1, ftell(fptr1));
		fclose(fptr1);
		fclose(fptr2);
	}
	else {
		printf("could not open file %s", filename1);
	}
}