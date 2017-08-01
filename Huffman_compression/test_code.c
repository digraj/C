#include <stdio.h>
#include <stdlib.h>

#define CHAR_SIZE 8

void print_char_in_bits(char number)
{
	int i;
	unsigned char mask = 1 << (CHAR_SIZE - 1);
	printf("mask : %d\n", mask) ;
	for (i = 0; i < CHAR_SIZE; i++) {
		// need to form a mask
		// mask = 1;
		// mask <<= (CHAR_SIZE - i - 1);
		int result = mask & number;
		if (result == 0) {
			printf("0");
		} else {
			printf("1");
		}
		mask >>= 1;
	}
}

int main(int argc, char *argv[])
{
	FILE *fptr;
	if (argc < 2) {
		fprintf(stderr, "error\n");
		return EXIT_FAILURE;
	}

	fptr = fopen(argv[1], "r");

	if (fptr == NULL) {
		fprintf(stderr, "error\n");
		return EXIT_FAILURE;
	}

	fseek(fptr, 0, SEEK_END);
	long fsize = ftell(fptr);
fprintf(stderr, "file size: %d\n", fsize) ;
	int n_int = fsize/sizeof(int);
fprintf(stderr, "n_int: %d\n", n_int) ;
	int *int_array = (int *)malloc(sizeof(int)*n_int);
	if (int_array == NULL) {
		fclose(fptr);
		fprintf(stderr, "error\n");
		return EXIT_FAILURE;
	}
	fseek(fptr, 0, SEEK_SET);
	int n_read = fread(int_array, sizeof(int), n_int, fptr);
	if (n_read != n_int) {
		free(int_array);
		fclose(fptr);
		fprintf(stderr, "error\n");
		return EXIT_FAILURE;
	}
	int i;
	for (i = 0; i < n_int; i++) {
		printf("this : %d \n", int_array[i]);
	}
	printf("\n");

	for (i = 0; i < n_int; i++) {
		printf("now this : %x \n", int_array[i]);
		printf("now this in C: %c \n", int_array[i]);	
	}
	printf("\n");

	char *char_array = (char *)&(int_array[0]);
	printf("random shit: %s\n", char_array) ;
	int n_char = n_int * sizeof(int);
	for (i = 0; i < n_char; i++) {
		printf("%c", char_array[i]);
	}
printf("\n") ;
	for (i = 0; i < n_char; i++) {
		print_char_in_bits(char_array[i]);
		printf("\n");
	} 
	free(int_array); 
	fclose(fptr);
	return EXIT_SUCCESS;
}

