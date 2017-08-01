#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include "huffunhuff.h"

int unhuff (char * file_input, char * file_output) ;

int main(int argc, char ** argv) 
{
	char * filename_input = argv[1] ;
	
	unhuff(filename_input, "output.unhuff") ;
	return EXIT_SUCCESS ;
}

void treeorder(tree *root, FILE * output_file)
{
   	if (root == NULL) {
      		return;
   	}

	if ((root->left == NULL) && (root->right == NULL)) {
		//fprintf(stderr, "1") ;
		//fprintf(stderr, "%c", root->data) ;
	}
	else {
		//fprintf(stderr, "0") ;
	}
	treeorder(root->left, output_file);
   	treeorder(root->right, output_file);	
}


int tree_i  ;
int tree_cnt  ;

tree * treebuilder (int * characters, int counter) 
{
//	if ((head->left != NULL) && (head->right != NULL)) {
//		return head ;
//	}	
//	if (tree_cnt == counter) {
//		return head ;
//	}
	tree * head = (tree *) malloc (sizeof (tree) ) ;
	head->left = NULL ;
	head->right = NULL ;
	if (characters[tree_i] == '1') {
		head->data = characters[tree_i + 1] ;
//				fprintf(stderr, "\n%c\n", characters[tree_i + 1]) ;			
		tree_i += 2 ;	
		tree_cnt++ ;			
	}
	else if (characters[tree_i] == '0') {
		tree_i += 1 ;
	
		head->left = treebuilder (characters, counter) ;
		head->right = treebuilder (characters, counter) ;
	}

	return head ;
}
	

tree * treebuilder_help (char * file_input)
{ 
	FILE * input = fopen(file_input, "r") ;

	fseek(input, 0, SEEK_END) ;

	int size = ftell(input) ;
	int i = 0 ;
	int * characters = (int *) calloc (size, sizeof(int)) ;

	fseek(input, 0, SEEK_SET) ;
	
	int cnt = 0 ;
	int counter = 0;

	while ((cnt = fgetc(input)) != EOF) {
		characters[i] = cnt ;
//		fprintf(stderr, "%c", characters[i]) ;
		i++ ;		
		if (cnt == 49) {
			counter++ ;
		}
	}
	tree_i = 0 ;
	tree * head = NULL ; 

	head = treebuilder (characters, counter) ;

	return head ;
}
 
int unhuff (char * file_input, char * file_output)
{
	FILE * input = fopen(file_input, "r") ;	
	fseek(input, 0, SEEK_END) ;
	int file_size = ftell(input) ;
	fseek(input, 0, SEEK_SET) ;

	char * characters = (char *) calloc (file_size, sizeof(char)) ;

	FILE * output = fopen("file_aux", "w") ;
	
	int cnt ;
	int j = 0 ;
	int i ;
	unsigned char temp = 0X00;

	while ((cnt = fgetc(input)) != EOF) {
		characters[j] = cnt ;
		j++ ;	
	}
		
	unsigned char chr ;
	unsigned char temp_2 = 0X00;
	int chk1 = 1 ;	
	int counter = 7 ;
	int quit = 0 ;

	i = 0 ;
	chr = characters[i] ;
	while (i < j) {		
		if (chk1 == 1) {			
			while (temp != 0x01) {
				temp = chr >> counter ;
				if (chr == 0xFF) {
					quit = 1 ; 
					fprintf(output, "0") ;
					break ;
				}	
				if (counter == 0) {
					counter = 7 ;
					i++ ;
					chr = characters[i] ;
				}
				else 				
					counter-- ;
				if (temp != 0x01) {
					fprintf(output, "0") ;
				} 
			}
			if (quit == 1) {
				break ;
			}
			fprintf(output, "1") ;
			chk1 = 0 ;
			counter = 7 - counter ;
			temp_2 = chr << counter ;
			i++ ;
			chr = characters[i] ;
		}
		else {
			temp = chr >> (8 - counter) ;
			temp = temp | temp_2 ;
			fprintf(output, "%c", temp) ;
			chk1 = 1 ;		
			chr = chr << (counter) ;
			chr = chr >> (counter) ;
			counter = 7 - counter ;
			temp = temp & 0x00 ;
		}		
	}

	FILE * file = fopen("file", "w") ;
	free(characters) ;
	fclose(input) ;
	fclose(output) ;

	tree * head = treebuilder_help ("file_aux") ;

//	treeorder(head, file) ;
	fclose(file) ;

	FILE * final_input = fopen(file_input, "r") ;
 
	fseek(final_input, 0, SEEK_SET) ;	

	while ((cnt = fgetc(final_input)) != 0xFF) {
	}
	
	int size = ftell(final_input) ;

	file_size = file_size - size + 1 ;

	char * nums = (char *) calloc (file_size, sizeof(char)) ;

	counter = 7 ;
	i = 0 ;

	while ((cnt = fgetc(final_input)) != EOF) {
		nums[i] = cnt ;
		i++ ;
	}

	j = 0 ;

	int * traverse = (int *) calloc (file_size * 8, sizeof(int)) ;
	int k = 0 ;

	while (j < i) {
		counter = 7 ;
		while (counter != 0) {
			temp = nums[j] << (7 - counter) ;
			temp = temp >> (7 - counter) ;
			temp = temp >> counter ;
			if (temp == 0x00) {
				traverse[k] = 0 ;
//				fprintf(stderr, "0") ;
			}
			else if (temp == 0x01) {
				traverse[k] = 1 ;
//				fprintf(stderr, "1") ;
			}
			k++ ;
			counter-- ;
		}
		j++ ;
	}		
//fprintf(stderr, "\n");
//	for (j = 0; j < k; j++ ) {
//		fprintf(stderr, "%d", traverse[j]) ;
//	}

//	fprintf(stderr, "\n\n") ;
	j = 0 ;
	
	tree * node = head ;	
	tree * headp = head ;

	treeorder(head, file) ;
//fprintf(stderr, "\n");

	FILE * final = fopen(file_output, "w") ;	
	
	while (j < k + 3) {
		if ((node->left == NULL) && (node->right == NULL)) {		
			fprintf(final, "%c", node->data) ;
			node = headp ;
		}
		else {			
			if (traverse[j] == 0) {
				node = node->left ;	
			}
			else if (traverse[j] == 1) {
				node = node->right ;
			}
			j++ ;		
		}	
	}	
	
	fprintf(final, "\n") ;	

//fprintf(stderr, "\n");
	tree_cnt = k ;
	tree_i = 0 ;
	return 0 ;
} 

