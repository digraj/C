#include <stdio.h>
#include <stdlib.h>

int main (int argc, char ** argv) 
{
	FILE * input = fopen("Check_100000", "r+") ;
	fseek(input, 0, SEEK_SET) ;	
	int cnt ;
	
	while ((cnt = fgetc(input)) != EOF) {
		if (cnt == 32) {
			fseek(input, -1, SEEK_CUR) ;
			fputc('\n', input) ;
		}
	}

	fclose(input) ;
	
	return 0 ;
}
	
