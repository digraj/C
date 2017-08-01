#include <stdio.h>
#include <stdlib.h>
#define SIZE 10


typedef struct lnode {
	struct lnode * next;
	int label;
} lnode;

void reverse_list () {
	lnode * head = (lnode *) malloc (sizeof(lnode));
	head->label = 0;
	lnode * next;
	lnode * original = head;
	int i = 0;
	while (i < SIZE) {
		next = malloc (sizeof(lnode));
		next->label = i ;
		head->next = next;
		head = next;
		i++;
	}

	head->next = NULL;
	head = original;

	
	i = 0;
	while (i < SIZE-1) {
		printf("%d->", head->next->label);
		head = head->next;
		i++;
	}
	
	lnode * last = head->next;
	printf("%d->NULL\n\n", last->label);

	lnode * prev = original->next;
	lnode * curr = NULL;
	next = prev->next;	
	
	prev->next = NULL;
	curr = next;
	while (curr != NULL) {
			curr = next;
			if (curr == NULL) break;
			next = next->next;
			curr->next = prev;
			prev = curr;
	}

	
	head = prev;
	
	i = 0;
	
	while (i < 9) {
		printf("%d->", head->label);
		prev = head;
		head = head->next;
		free(prev);
		i++;
	}
	
	last = head;
	printf("%d->NULL\n\n", last->label);	
	free(last);
	free(original);
	
}
int main ()
{
	reverse_list();
	return EXIT_SUCCESS;
}