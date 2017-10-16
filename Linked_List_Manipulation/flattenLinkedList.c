#include <stdio.h>
#include <stdlib.h>

typedef struct lnode {
	struct lnode * next;
	struct lnode * down;
	int value;
	int visited;
} lnode;


void print_structure(lnode * head)
{
	printf("%d -> %d -> %d -> %d ->NULL\n", head->value, head->next->value, head->next->next->value, head->next->next->next->value) ;
	printf("|     |     |     |\n");
	printf("V     V     V     V\n");
	printf("%d    %d    %d    %d\n", head->down->value, head->next->down->value, head->next->next->down->value, head->next->next->next->down->value) ;
	printf("|     |     |     |\n");
	printf("V     V     V     V\n");
	printf("%d   NULL   %d    %d\n", head->down->down->value, head->next->next->down->down->value, head->next->next->next->down->down->value) ;	
	printf("|           |     |\n");
	printf("V           V     V\n");
	printf("%d         NULL   %d\n", head->down->down->down->value, head->next->next->next->down->down->down->value) ;	
	printf("|                 |\n");
	printf("V                 V\n");
	printf("NULL            NULL\n\n\n");
	printf("Done Printing\n\n");
	return ;
}

lnode * creator()
{
	lnode * head = malloc (sizeof(lnode));
	head->next = malloc (sizeof(lnode));
	head->next->next = malloc (sizeof(lnode));
	head->next->next->next = malloc (sizeof(lnode));
	head->next->next->next->next = NULL;
	
	head->down = malloc (sizeof(lnode));
	head->down->down = malloc (sizeof(lnode));
	head->down->down->down = malloc (sizeof(lnode));
	head->down->down->down->down = NULL;
	
	head->next->down = malloc (sizeof(lnode));
	head->next->down->down = NULL;
	
	head->next->next->down = malloc (sizeof(lnode));
	head->next->next->down->down = malloc (sizeof(lnode));
	head->next->next->down->down->down = NULL;
	
	head->next->next->next->down = malloc (sizeof(lnode));
	head->next->next->next->down->down = malloc (sizeof(lnode));
	head->next->next->next->down->down->down = malloc (sizeof(lnode));
	head->next->next->next->down->down->down->down = NULL;
	
	head->value = 5;
	head->next->value = 10;
	head->next->next->value = 19;
	head->next->next->next->value = 28;
	
	head->down->value = 7;
	head->down->down->value = 8;
	head->down->down->down->value = 30;
	
	head->next->down->value = 20;
	
	head->next->next->down->value = 22;
	head->next->next->down->down->value = 50;

	head->next->next->next->down->value = 6; //35
	head->next->next->next->down->down->value = 40;
	head->next->next->next->down->down->down->value = 45;

	head->visited = -1;
	head->next->visited = -1;
	head->next->next->visited = -1;
	head->next->next->next->visited = -1;
	
	head->down->visited = -1;
	head->down->down->visited = -1;
	head->down->down->down->visited = -1;
	
	head->next->down->visited = -1;
	
	head->next->next->down->visited = -1;
	head->next->next->down->down->visited = -1;

	head->next->next->next->down->visited = -1;
	head->next->next->next->down->down->visited = -1;
	head->next->next->next->down->down->down->visited = -1;
	
	return head;
}


lnode * flatten(lnode * head)
{
	lnode * original = head;
	lnode * nextPtr = head;
	lnode * downPtr = head->down;
	lnode * ptr = head;
	int last = 0;
	while (1) {
		while (downPtr != NULL) {
			ptr = downPtr;
			downPtr->next = downPtr->down;
			downPtr->down = NULL;
			downPtr = downPtr->next;
		}
		if (head->next == NULL) {
			head->next = head->down;
			break;
		}
		nextPtr = head->next;
		ptr->next = nextPtr;
		head->next = head->down;
		head = nextPtr;
		downPtr = nextPtr->down;
	}
	
	return original;
}
	
lnode * sortLinkedList(lnode * head) 
{

	int sorted = 0;
	lnode * original = head;
	lnode * prev = head;
	lnode * next = head->next;
	lnode * curr = head->next;
	lnode * next_prev = head;
	lnode * next_next = next->next;
	while (1) {
		while (next != NULL) {
			if (next->value < curr->value) {
				prev->next = next;
				next->next = curr;
				next_prev->next = next_next;
				break;
			}
			printf("%d next\n", next->value); 
			next_prev = next;
			next = next->next;
			if (next != NULL)
				next_next = next->next;	
		}
		prev = curr;
		curr = curr->next;
		next = curr;
		if (curr != NULL)
		printf("%d Current %d Prev\n", curr->value, prev->value);
		if (curr == NULL) {
				printf("Breaking");
				break;
		}
//		if (next != NULL)
		next = next->next;
		next_prev = prev;
		if (next != NULL) {
				next_next = next->next;
//				printf("%d %d %d \n", prev->value, curr->value, next->value);
		}
	}
	while (original != NULL) {
		printf("%d ", original->value) ;
		original = original->next;
	}
	return NULL;
}

int main()
{
	lnode * head = creator();
	print_structure(head);

	lnode * flatten_unsorted = flatten(head);
	lnode * sorted = sortLinkedList(flatten_unsorted);
	return EXIT_SUCCESS;
}