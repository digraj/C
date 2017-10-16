#include <stdio.h>
#include <stdlib.h>

typedef struct list {
	struct list * next;
	int val;
} list;

void printLinkedList(list * head)
{
	if (head == NULL) 
		printf("Head is NULL");
	list * myList = head;
	while (myList != NULL) {
		printf("%d->", myList->val);
		myList = myList->next;
	}
	
	printf("NULL\n\n");
}

void addNode(int val, list * head) {
	list * newNode = malloc(sizeof(list));
	newNode->next = NULL;
	newNode->val = val;
	list * curr = head;
	while (curr->next != NULL) {
		curr = curr->next;
	}
	curr->next = newNode;
}
		
void removeNode(int val, list * head) 
{
	printf("Head Value: %d\n", head->val);
	list * curr = head;
	list * prev = curr;
	list * next = curr->next;
	
	while (curr->val != val) {
		prev = curr;
		curr = curr->next;
		next = curr->next;
	}
	
	prev->next = next;
	if (curr != NULL)
		free(curr);
}
		
list * createList() {
	int val = 5;
	list * head = malloc (sizeof(list));
	head->next = NULL;
	head->val = 5;
	val = 6;
	printf("Adding First Node\n\n");
	addNode(6, head);
	addNode(8, head);
	addNode(11, head);
	addNode(2, head);
	addNode(3, head);
	printLinkedList(head);
	removeNode(11, head);
	printLinkedList(head);
	removeNode(6, head);
	printLinkedList(head);
	removeNode(2, head);
	printLinkedList(head);
	removeNode(3, head);
	printLinkedList(head);
	removeNode(8, head);
	printLinkedList(head);
	return head;
}


int main () 
{
	printf ("Starting Program\n");
	list * myList = createList();
	list * curr;
	while (myList != NULL) {
		curr = myList;
		myList = myList->next;
		free(curr);
	}
	
	return 0;
}