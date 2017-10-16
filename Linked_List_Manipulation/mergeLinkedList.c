#include <stdio.h>
#include <stdlib.h>


typedef struct lnode {
	struct lnode * next;
	int value;
} lnode;

int main () 
{
		int * list1 = (int *) malloc (sizeof(lnode)) ;
		
	