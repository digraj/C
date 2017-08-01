#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "huffunhuff.h"

int main(int argc, char ** argv) 
{
	char * filename_input = argv[1] ;
	char * filename_output1 = "file1" ;
	char * filename_output2 = "file2" ;
	char * filename_output3 = "output.huff" ;

	int ascii_collector[256] = {0};

	int rtn_val = File_256_lines(filename_input, ascii_collector) ;	
	if (rtn_val == -1) {
		return EXIT_FAILURE ;
	}

	rtn_val = priority_creator(filename_output1, ascii_collector, filename_output2) ;	
	if (rtn_val == -1) {
		return EXIT_FAILURE ;
	}	

	rtn_val = binary_creator(filename_output2, filename_output3, filename_input, filename_output1) ;
	if (rtn_val == -1) {
		return EXIT_FAILURE ;
	}

//	unhuff(filename_output3, filename_output4) ;
	return EXIT_SUCCESS ;
}

void postorder(lnode *root, FILE * output_file)
{
   	if (root == NULL) {
      		return;
   	}
//   	postorder(root->left, output_file);
//   	postorder(root->right, output_file);
	if ((root->left == NULL) && (root->right == NULL)) {
		fprintf(output_file, "1") ;
		fprintf(output_file, "%c", *(const unsigned char *)root->ptr_char) ;
	}
	else {
		fprintf(output_file, "0") ;
	}
	postorder(root->left, output_file);
   	postorder(root->right, output_file);	
}

void lnode_destroy(lnode *root)
{
	if (root == NULL) 
      		return;
   	lnode_destroy(root->left);
   	lnode_destroy(root->right);
   	free(root);
}

long int int_cmp(const void *p1, const void *p2)
{
   	return *(const long int *)p1 - *(const long int *)p2 ;
}

unsigned char char_cmp(const void *p1, const void *p2)
{
   	return *(const unsigned char *)p1 - *(const unsigned char *)p2;
}

int File_256_lines(char * filename_input, int * ascii_collector)
{
	FILE * input_file = fopen(filename_input, "r") ;
	if (input_file == NULL) {
		return -1;	
	}

	int fnt = 0;
	fseek(input_file, 0, SEEK_SET) ;

	if ((fnt = fgetc(input_file)) == EOF) {
		return -1 ;
	}

	fseek(input_file, 0, SEEK_SET) ;
	
	int cnt = 0;
	
	while ((cnt = fgetc(input_file)) != EOF) {
		ascii_collector[cnt] = ascii_collector[cnt] + 1 ; 
	}

	fclose(input_file) ;

	return 0 ;
}

int priority_creator(char * filename_output_huff, int *ascii_collect, char * filename_output_char) 
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

	unsigned char * char_collector = (unsigned char *)calloc(counter + 1, sizeof(unsigned char)) ;	
	long int * ascii_collector = (long int *)calloc(counter, sizeof(long int)) ;
	cnt = 0 ;
	while (cnt < 256) {
		if (ascii_collect[cnt] != 0) {
			char_collector[i] = cnt;
			ascii_collector[i] = ascii_collect[cnt];
			i++ ;	
		}
		cnt++ ;
	}	
	
	for (i = 0; i < counter; i++) {
		new_node = PQ_enqueue(&head, &ascii_collector[i], &char_collector[i], int_cmp, char_cmp, NULL, NULL) ;
	}

	lnode * huffman_bst = huffman_tree(head) ;
	FILE * output_file = fopen(filename_output_huff, "w") ;
	if (output_file == NULL) {
		lnode_destroy(huffman_bst);
		free(ascii_collector) ;
		free(char_collector) ;
		return -1;
	}
	FILE * output_file2 = fopen(filename_output_char, "w") ;
	if (output_file2 == NULL) {
		fclose(output_file) ;
		lnode_destroy(huffman_bst);
		free(ascii_collector) ;
		free(char_collector) ;
		return -1;
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

	return 0;
}

void tree_traversal(lnode * huffman_bst, FILE * output_file, int * l_r, long int depth, long int dir)
{
	if (huffman_bst == NULL) {
      		return;
   	}
	if (dir != -1) {
		l_r[depth] = dir ;
	}
	
	int i = 1;
	if (huffman_bst->left == NULL) {	
		fprintf(output_file, "%c:", *(unsigned char *)huffman_bst->ptr_char) ;
		for (i = 0; i <= depth; i++) {
			fprintf(output_file, "%d", l_r[i]) ;
		}
		fprintf(output_file, "\n") ;
	}
	depth = depth + 1;
   	tree_traversal(huffman_bst->left, output_file, l_r, depth, 0);
   	tree_traversal(huffman_bst->right, output_file, l_r, depth, 1);
}
		
lnode *PQ_enqueue(lnode **pq, const void *new_object, const unsigned char *new_object_1, long int (*cmp_fn_1)(const void *, const void *), unsigned char (*cmp_fn_2)(const void *, const void *), lnode * left, lnode * right)
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
//			printf("%c %c %d\n", *(const int *)curr->ptr_char, *(const int *)new_object_1, *(const int *)curr->ptr_char - *(const int *)new_object_1) ;
			if (cmp_fn_2(curr->ptr_char, new_object_1) < 0) {
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

lnode *lnode_construct(const void *ptr, const unsigned char *ptr_char, lnode * left, lnode * right)
{
   	lnode *ret_node = (lnode *)malloc(sizeof(lnode));
   	if (ret_node == NULL) {
      		return NULL;
   	}
   	ret_node->ptr = (void *)ptr;
	ret_node->ptr_char = (unsigned char *)ptr_char;
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
	
	long int label = head->label + head->next->label;
	
	lnode * head_next = head ;
	lnode * head_next_next = head->next ;

	head = head->next->next ;

	head_next->next = NULL ;
	(head_next_next)->next = NULL ;

	unsigned char char_val = *(unsigned char *)head_next->ptr_char + *(unsigned char *)head_next_next->ptr_char ;
	lnode * sorted = PQ_enqueue(&head, &label, &char_val, int_cmp, char_cmp, head_next, head_next_next) ;
	
	sorted->label = sorted->left->label + sorted->right->label ;

	if (head->next == NULL) {
		return head;
	}

	lnode * new_node = huffman_helper(head) ;	
	return new_node ;
}

int binary_creator(char * filename_input, char * filename_output, char * input_2, char * input_3) 
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
	int cnt_preserve = -1 ;
	while ((cnt = fgetc(input_file)) != EOF) 
	{	
		if ((cnt == 49) && (cnt_preserve != cnt)) {	
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
		else if ((cnt == 48) && (cnt_preserve != 49)) {	
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
		else {
			temp = cnt << (CHAR_SIZE - counter + 1) ;
			cnt = cnt >> (counter - 1) ;
			mask_1 = mask | cnt ;			
			fprintf(output_file, "%c", mask_1) ;
		}
		if (cnt == 49) {		
			cnt_preserve = cnt ;
		}
		else {
			cnt_preserve = -1 ;
		}	
	}

	if (counter != 1) {
		mask = 0x00 ;
		temp = temp | mask ;		
		fprintf(output_file, "%c", temp) ;
	}

	fclose(input_file) ;	
	
	fprintf(output_file, "%c", 0xFF) ;
	
	FILE * input = fopen(input_2, "r") ;

	FILE * file2 = fopen(input_3, "r") ;

	fseek(input, 0, SEEK_SET) ;

	int cnt_2 = 0 ;
	int i = 0 ;
	while ((cnt = fgetc(input)) != EOF) {
		while ((cnt_2 = fgetc(file2)) != cnt) {
		}
		fseek(file2, 1, SEEK_CUR) ;
		while ((cnt_2 = fgetc(file2)) != '\n') { 		
//			fprintf(stderr, "%c", cnt_2) ; 
			i++ ;
		}
		fseek(file2, 0, SEEK_SET) ;
	}				
	
	i = (((i/8) + 1) * 8);

	int * numbers = (int *) calloc (i, sizeof(int)) ;
	fseek(input, 0, SEEK_SET) ;
	fseek(file2, 0, SEEK_SET) ;
	i = 0 ;

	while ((cnt = fgetc(input)) != EOF) {
		while ((cnt_2 = fgetc(file2)) != cnt) {
		}
		fseek(file2, 1, SEEK_CUR) ;
		while ((cnt_2 = fgetc(file2)) != '\n') { 		
			numbers[i] = cnt_2 - 48 ;
			i++ ;
		}
		fseek(file2, 0, SEEK_SET) ;
	}	

	
	int j = 0 ;

	unsigned char bit8 = 0x00 ;
	counter = 7 ;
//	fprintf(stderr, "\n%d\n", i) ;
	for (j = 0; j < i; j++) {
		if (numbers[j] == 1) {
			bit8 = bit8 | (0x01 << counter) ;
//			fprintf(stderr, "%x,  %d,   %d\n", bit8, j, counter) ;
		}
		counter-- ;

		if (counter == 0) {
			fprintf(output_file, "%c", bit8) ;
//			fprintf(stderr, "\n") ;
			counter = 7 ;
			bit8 = bit8 & 0x00 ;		
		}		
	}

//	fprintf(stderr, "\n\n") ;
	free(numbers) ;
	fclose(input) ;
	fclose(file2) ;
	fclose(output_file) ;

	return 0;
}

