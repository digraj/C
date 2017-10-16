#include <stdio.h>
#include <stdlib.h>
#define MAXLEN 10

void printArr(int array[]) 
{
	int i;

	for (i = 0; i < MAXLEN; i++) {
		printf("%d ", array[i]);
	}

	printf("\n");
}

void bubbleSort(int * a) 
{
	int i, j;
	int temp;
	for (i = 0; i < MAXLEN; i++) {
		for (j = 0; j < MAXLEN - 1; j++) {
			if (a[j] > a[j + 1]) {
				temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;
			}
		}
	}
}

int main () 
{
	int array[MAXLEN] = {44, 2, 30, 7, 10, 6, 21, 4, 92, 14};
	printArr(array);
	bubbleSort(array);
	printArr(array);

	return EXIT_SUCCESS;
}