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


int partition(int * arr, int low, int high)
{

	int pivot = arr[high];	//The rightmost element is the pivot
	int i = low - 1;		//Index of the lowest element
	int j;
	int temp;
	for (j = low; j < high; j++) {
		if (arr[j] <= pivot) {
			i++;
			temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
		}
	} 
	temp = arr[i + 1];
	arr[i + 1] = arr[high];
	arr[high] = temp;

	return i + 1;
}

void quickSort(int * arr, int low, int high)
{
	if (low < high)
	{
		int parIndex = partition(arr, low, high);

		quickSort(arr, low, parIndex - 1);
		quickSort(arr, parIndex + 1, high);
	}
	return;
}

int main () 
{
	int array[MAXLEN] = {44, 2, 30, 7, 10, 6, 21, 4, 92, 14};
	printArr(array);

	quickSort(array, 0, MAXLEN - 1);

	printArr(array);

	return EXIT_SUCCESS;
}