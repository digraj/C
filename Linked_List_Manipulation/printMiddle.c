#include <stdio.h>
#include <stdlib.h>
#define SIZE 10


typedef struct lnode {
	struct lnode * next;
	int label;
} lnode;

void free_list(lnode * head) {
	lnode * prev = head;
	while (head != NULL) {
		prev = head;
		head = head->next;
		free(prev);
	}
}

void print_middle(lnode * head) {
	lnode * original = head;
	int i = 0;
	while (head != NULL) {
		i++;
		head = head->next;
	}
	
	int middle = (i/2) + (i+1)%2;
	head = original;

	while (middle >= (0 + (i+1)%2)) {
		head = head->next;
		middle--;
	}
	printf("Middle: %d\n\n", head->label);
	
	free_list(original);
	
}

int main () {
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

	i = 0;
	head = original;
	while (i < SIZE-1) {
		printf("%d->", head->next->label);
		head = head->next;
		i++;
	}
	
	lnode * last = head->next;
	printf("%d->NULL\n\n", last->label);
	
	head->next = NULL;
	head = original;
	lnode * middle = head;
	print_middle(middle);
	
	free(next);
	
	return EXIT_SUCCESS;
}