#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct _lnode {
	struct _lnode * next;
	int value ;
} lnode ;
	

int adjacent_list (char * filename) 
{
	FILE * input = fopen(filename, "r") ;
	if (input == NULL) {
		return -1 ;
	}
	FILE * out = fopen("out", "w") ;
	if (out == NULL) {
		return -1 ;
	}

	fseek(input, 0, SEEK_SET) ;

	int cnt ;
	int vertices = 0;

	while ((cnt = fgetc(input)) != 32) {
		vertices *= 10 ;
		vertices = vertices + (cnt - 48) ;
	}

	fseek(input, 0, SEEK_SET) ;	

	int line_cnt = 0;
	
	while (line_cnt != vertices + 1) {
		cnt = fgetc(input) ;
		if (cnt == '\n') {
			line_cnt++ ;
		}
	}

	lnode ** list = (lnode **) calloc (vertices, sizeof(lnode *)) ;

	int num = 0 ;
	int val = 0 ;
	int * numbers = (int *) calloc (vertices , sizeof(int)) ;
	lnode * head = list[0];	
	lnode * cur = NULL;

	while ((cnt = fgetc(input)) != EOF) {
		while ((cnt = fgetc(input)) == 32) {
		}
		fseek(input, -1, SEEK_CUR) ;
		while ((cnt = fgetc(input)) != 32) {
			num *= 10 ;
			num += cnt - 48 ;
		}
		while ((cnt = fgetc(input)) == 32) {
		}
		numbers[num] = 1 ;
		if (list[num] == NULL) { 		
			list[num] = (lnode *) calloc (1, sizeof(lnode)) ;
		}		
		head = list[num] ;
		cur = head ;
		while (head != NULL) {
			cur = head ;
			head = head->next ;		  	
		}		

		head = cur ;
		fseek(input, -1, SEEK_CUR) ;
		while ((cnt = fgetc(input)) != '\n') {
			val *= 10 ;
			val += cnt - 48 ;
		}

		head->value = val ;
		cur = (lnode *) calloc (1, sizeof(lnode)) ;
		head->next = cur ;
		head->next->value = -1 ;		
		head->next->next = NULL ;
		num = 0 ;	
		val = 0 ;	
	}	
			
	//PRINT

	int i = 0 ;
	
	for (i = 0 ; i < vertices; i++) {
		if (numbers[i] == 1) {
			head = list[i] ;
			if (head != NULL) {		
				fprintf(out, "%d: ", i) ;
		
				while (head != NULL) {
					if (head->value != -1) {
						fprintf(out, "%d ", head->value) ;
					}				
					head = head->next ;		  	
				}
				fprintf(out, "\n") ;
			}
		}	
	}
	
	fclose(input) ;

	for (i = 0 ; i < vertices; i++) {
		if (numbers[i] == 1) {
			head = list[i] ;	
			while (head != NULL) {
				cur = head ;				
				head = head->next ;
				free(cur) ;
			}
		}
	} 	
	
	free(list) ;
	free(numbers) ;

	return 0 ;
}

int main (int argc, char ** argv) 
{
	char * filename_1 = argv[1] ;

	int val = adjacent_list(filename_1) ;
	if (val == -1) {
		return EXIT_FAILURE ;
	}

	return EXIT_SUCCESS ;
}


