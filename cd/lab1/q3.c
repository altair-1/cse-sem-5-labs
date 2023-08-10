#include <stdio.h>

void main() {
	FILE * fptr1, * fptr2, * fptr3;
	char filename1[100], filename2[100], filename3[100];
	printf("enter first filename to open for reading: ");
	scanf("%s", filename1);
	printf("enter second filename to open for reading: ");
	scanf("%s", filename2);
	printf("enter filename to open for writing: ");
	scanf("%s", filename3);
	fptr1 = fopen(filename1, "r");
	fptr2 = fopen(filename2, "r");
	fptr3 = fopen(filename3, "w+");

	if ((fptr1 != NULL) && (fptr2 != NULL)) {
		char ch1 = fgetc(fptr1), ch2;
		int flag = 1;
		while (ch1 != EOF && ch2 != EOF) {
			if (flag) {
				while (ch1 != EOF && ch1 != '\n') {
					fputc(ch1, fptr3);
					ch1 = fgetc(fptr1);
				}
				fputc('\n', fptr3);
				ch2 = fgetc(fptr2);
				flag = 0;
			}
			else {
				while (ch2 != EOF && ch2 != '\n') {
					fputc(ch2, fptr3);
					ch2 = fgetc(fptr2);
				}
				fputc('\n', fptr3);
				ch1 = fgetc(fptr1);
				flag = 1;
			}
		}
		while (ch1 != EOF) {
			fputc(ch1, fptr3);
			ch1 = fgetc(fptr1);
		}
		while (ch2 != EOF) {
			fputc(ch2, fptr3);
			ch2 = fgetc(fptr2);
		}
		printf("files merged\n");
		fclose(fptr1);
		fclose(fptr2);
		fclose(fptr3);
	}
	else {
		if (fptr1 == NULL)
			printf("could not open file %s", filename1);
		else
			printf("could not open file %s", filename2);
	}
}