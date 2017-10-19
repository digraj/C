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

void merge(int arr[], int low, int mid, int high)
{
	int n, m, i, k , j;

	n = mid - low + 1;
	m = high - mid;

	int temp1[n], temp2[m];

	for (i = 0; i < n; i++) {
		temp1[i] = arr[low + i];
	for (j = 0; j < m; j++)
		temp2[j] = arr[mid + 1 + j];
	}

	i = 0;
	j = 0;
	k = low;

	while (i < n && j < m) {
		if (temp1[i] <= temp2[j]) {
			arr[k] = temp1[i];
			i++;
		}
		else {
			arr[k] = temp2[j];
			j++;
		}
		k++;
	}

	while (i < n)
	{
		arr[k] = temp1[i];
		i++;
		k++;
	}

	while (j < m)
	{
		arr[k] = temp2[j];
		j++;
		k++;
	}
	return;
}

void mergeSort(int array[], int low, int high) 
{
	//printArr(array);
	int mid;
	if (low < high) {
		mid = (low + high) / 2;
		mergeSort(array, low, mid);
		mergeSort(array, mid + 1, high);
		merge(array, low, mid, high);
	} 
}

int main () 
{
	int array[MAXLEN] = {44, 2, 30, 7, 10, 6, 21, 4, 92, 14};
	printArr(array);
	mergeSort(array, 0, MAXLEN - 1);
	printArr(array);

	return EXIT_SUCCESS;
}