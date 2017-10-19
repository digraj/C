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

void insertionSort(int * arr) {
	int i, j, temp;
	for (i = 0; i < MAXLEN; i++) {
		for (j = 0; j < i; j++) {
			if (arr[j] > arr[i]) {
				temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
		}
	}

	return;
}

int main () 
{
	int array[MAXLEN] = {44, 2, 30, 7, 10, 6, 21, 4, 92, 14};
	printArr(array);

	insertionSort(array);

	printArr(array);

	return EXIT_SUCCESS;
}