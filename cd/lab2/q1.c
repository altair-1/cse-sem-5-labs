#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE *fa, *fb;
	int ca, cb;
	fa = fopen("src.txt","r");
	if(fa == NULL){
		printf("cannot open file\n");
		exit(0);
	}
	fb = fopen("dst.txt","w");
	ca = getc(fa);
	while(ca != EOF){
		if(ca == ' ' || ca == '\t'){
			putc(' ',fb);
			while(ca == ' ' || ca == '\t')
				ca = getc(fa);
		}
		if(ca == '/'){
			cb = getc(fa);
			if(cb == '/'){
				while(ca != '\n')
					ca = getc(fa);
			}
			else if(cb == '*'){
				do{
					while(ca != '*')
						ca = getc(fa);
					ca = getc(fa);
				} while(ca != '/');
			}
			else{
				putc(ca, fb);
				putc(cb, fb);
			}
		}
		else{
			putc(ca, fb);
			ca = getc(fa);
		}
	}
	printf("replaced blank spaces and tabs by single space\n");
	fclose(fa);
	fclose(fb);
	return 0;
}