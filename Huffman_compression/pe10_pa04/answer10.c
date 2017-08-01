#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include "answer04.h"

//ADD RETURN -1

void postorder(lnode *root, FILE * output_file)
{
   	if (root == NULL) {
      		return;
   	}
   	postorder(root->left, output_file);
   	postorder(root->right, output_file);
	if ((root->left == NULL) && (root->right == NULL)) {
		fprintf(output_file, "1") ;
		fprintf(output_file, "%c", *(const int *)root->ptr_char) ;
	}
	else {
		fprintf(output_file, "0") ;
	}	
}

void lnode_destroy(lnode *root)
{
	if (root == NULL) 
      		return;
   	lnode_destroy(root->left);
   	lnode_destroy(root->right);
   	free(root);
}

static int int_cmp(const void *p1, const void *p2)
{
   return *(const int *)p1 - *(const int *)p2;
}

int File_256_lines(char * filename_input, int *ascii_collector)
{
	FILE * input_file = fopen(filename_input, "r") ;
	if (input_file == NULL) 
		return -1;	
	fseek(input_file, 0, SEEK_SET) ;
	
	int cnt = 0;
	
	while ((cnt = fgetc(input_file)) != EOF) {
		ascii_collector[cnt] = ascii_collector[cnt] + 1 ; 
	}

	fclose(input_file) ;

	return 0 ;
}

void priority_creator(char * filename_output_huff, int *ascii_collect, char * filename_output_char) 
{
	lnode *head ;
	head = NULL;	
	lnode *new_node ;
	new_node = NULL;
	
	int cnt = 0;
	int counter = 0;
	int i = 0 ;
	while (cnt != 256) {
		if (ascii_collect[cnt] != 0) {
			counter++ ;
		}
		cnt++ ;
	}

	int * char_collector = (int *)calloc(counter + 1, sizeof(int)) ;	
	int * ascii_collector = (int *)calloc(counter, sizeof(int)) ;
	cnt = 0 ;
	while (cnt != 256) {
		if (ascii_collect[cnt] != 0) {
			char_collector[i] = cnt;
			ascii_collector[i] = ascii_collect[cnt];
			i++ ;	
		}
		cnt++ ;
	}	
	
	for (i = 0; i < counter; i++) {
		new_node = PQ_enqueue(&head, &ascii_collector[i], &char_collector[i], int_cmp, NULL, NULL) ;
	}

	//lnode_print(head, filename_output) ;
	lnode * huffman_bst = huffman_tree(head) ;
	FILE * output_file = fopen(filename_output_huff, "w") ;
	if (output_file == NULL) {
		lnode_destroy(huffman_bst);
		free(ascii_collector) ;
		free(char_collector) ;
		return ;
	}
	FILE * output_file2 = fopen(filename_output_char, "w") ;
	if (output_file2 == NULL) {
		fclose(output_file) ;
		lnode_destroy(huffman_bst);
		free(ascii_collector) ;
		free(char_collector) ;
		return ;
	}
	int * l_r = (int *)malloc(sizeof(int) * 256) ;
	int k = 0;
	for (k = 0; k < 256; k++) {
		l_r[k] = -1 ;
	}

	tree_traversal(huffman_bst, output_file, l_r, -1, -1) ;
	postorder(huffman_bst, output_file2) ;
	fprintf(output_file2, "0") ;
	fclose(output_file) ;
	fclose(output_file2) ;
	free(l_r) ;
	lnode_destroy(huffman_bst);
	free(ascii_collector) ;
	free(char_collector) ;

	return ;
}

void tree_traversal(lnode * huffman_bst, FILE * output_file, int * l_r, int depth, int dir)
{
	if (huffman_bst == NULL) {
      		return;
   	}
	if (dir != -1) {
		l_r[depth] = dir ;
	}
	
	int i = 1;
	if (huffman_bst->left == NULL) {	
		fprintf(output_file, "%c:", *(const int  *)huffman_bst->ptr_char) ;
		for (i = 0; i <= depth; i++) {
			fprintf(output_file, "%d", l_r[i]) ;
		}
		fprintf(output_file, "\n") ;
	}
	depth = depth + 1;
   	tree_traversal(huffman_bst->left, output_file, l_r, depth, 0);
   	tree_traversal(huffman_bst->right, output_file, l_r, depth, 1);
}
		
lnode *PQ_enqueue(lnode **pq, const void *new_object, const int *new_object_1, int (*cmp_fn_1)(const void *, const void *), lnode * left, lnode * right)
{
	if (new_object == NULL) {
		return NULL;
	}
	lnode *new_node = NULL ;
	if ((left != NULL) && (right != NULL)) {
		left->next = NULL ;
		right->next = NULL ;
		new_node = lnode_construct(new_object, new_object_1, left, right);
	}
	else {
		new_node = lnode_construct(new_object, new_object_1, NULL, NULL);
	}	
	if (new_node == NULL) {
		return NULL;
	}
	lnode dummy;
	dummy.next = *pq;
	lnode *prev = &dummy;
	lnode *curr = *pq;

	while (curr != NULL) {
		if (cmp_fn_1(curr->ptr, new_object) > 0) {
			break;
		}
		else if (cmp_fn_1(curr->ptr, new_object) == 0) {
			if (cmp_fn_1(curr->ptr_char, new_object) < 0) {
				break ;
			}
			else {
				prev = curr;
				curr = curr->next;
			}
		}  
		else {
			prev = curr;
			curr = curr->next;
		}
	}
	prev->next = new_node;
	new_node->next = curr;
	*pq = dummy.next;
	return new_node;		
}

lnode *lnode_construct(const void *ptr, const int *ptr_char, lnode * left, lnode * right)
{
   	lnode *ret_node = (lnode *)malloc(sizeof(lnode));
   	if (ret_node == NULL) {
      		return NULL;
   	}
   	ret_node->ptr = (void *)ptr;
	ret_node->ptr_char = (int *)ptr_char;
   	ret_node->next = NULL;
	ret_node->left = left ;
	ret_node->right = right ;
   	return ret_node;
}

lnode * huffman_tree(lnode * head) 
{
	lnode * temp = head ;
	while (temp != NULL) {
		temp->label = (*(const int *)temp->ptr) ;	
		temp = temp->next ;
	}

	lnode * new_node = huffman_helper(head) ;
	
	return new_node ;

}

lnode * huffman_helper(lnode * head) 
{
	if ((head->next == NULL)) {
		return head ;
	}
	
	int label = head->label + head->next->label;
	
	lnode * head_next = head ;
	lnode * head_next_next = head->next ;

	head = head->next->next ;

	head_next->next = NULL ;
	(head_next_next)->next = NULL ;

	int char_val = *(const int *)head_next->ptr_char + *(const int *)head_next_next->ptr_char ;
	lnode * sorted = PQ_enqueue(&head, &label, &char_val, int_cmp, head_next, head_next_next) ;
	
	sorted->label = sorted->left->label + sorted->right->label ;

	if (head->next == NULL) {
		return head;
	}

	lnode * new_node = huffman_helper(head) ;	
	return new_node ;
}

int binary_creator(char * filename_input, char * filename_output) 
{
	FILE * input_file = fopen(filename_input, "r") ;
	if (input_file == NULL) 
	{
		return -1 ;
	}
	
	FILE * output_file = fopen(filename_output, "w") ;
	if (input_file == NULL) 
	{
		fclose(input_file) ;
		return -1 ;
	}

	fseek(input_file, 0, SEEK_SET) ;

	int cnt = 0 ;
	unsigned char mask = 0 ;
	unsigned char mask_1 = 0 ;
	unsigned char temp = 0 ;
	int counter = 1 ;
	while ((cnt = fgetc(input_file)) != EOF) 
	{	
		if (cnt == 49) {	
			mask = 0x01 ;
			mask = 1 << (CHAR_SIZE - counter) ;
			if (counter > 1) {
				mask = mask | temp ;
			}
			counter++ ;	
			if (counter > 8) {
				counter = 1 ;
				temp = 0x00 ;
				fprintf(output_file, "%c", mask) ;
				mask_1 = 0x00 ;
				mask = 0x00 ;
			}
		}
		else if (cnt == 48) {	
			mask = 0x00 ;
			mask = mask | temp ;			
			counter++ ;
			if (counter > 8) {
				counter = 1 ;
				temp = 0x00 ;
				fprintf(output_file, "%c", mask) ;
				mask_1 = 0x00 ;
				mask = 0x00 ;
			}
		}
		else if ((cnt != 48) && (cnt != 49)) {
			temp = cnt << (CHAR_SIZE - counter + 1) ;
			cnt = cnt >> (counter - 1) ;
			mask_1 = mask | cnt ;
			fprintf(output_file, "%c", mask_1) ;
		}	
	}

	fclose(input_file) ;	
	fclose(output_file) ;
	
	return 0;
}
























