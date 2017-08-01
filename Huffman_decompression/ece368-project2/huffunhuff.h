#ifndef PA04_H
#define PA04_H 

#define CHAR_SIZE 8

typedef struct _lnode {
   	long int *ptr;
	void *ptr_char;
	long int label;
	struct _lnode *left;
   	struct _lnode *right;
   	struct _lnode *next;
} lnode;

typedef struct _tree {
	struct _tree *left ;
	struct _tree *right ;
	char  data ;
} tree ;

int File_256_lines(char * filename_input, int *ascii_collector) ;

//MAKES A PRIORITY QUEUE
lnode *PQ_enqueue(lnode **pq, const void *new_object, const unsigned char *new_object_1, long int (*cmp_fn_1)(const void *, const void *), unsigned char (*cmp_fn_2)(const void *, const void *), lnode * left, lnode * right);

//MALLOCS FOR A QUEUE
lnode *lnode_construct(const void *ptr, const unsigned char *ptr_char, lnode * left, lnode * right) ;

//FREE MEMORY
void destroy(void * prev) ;

void tree_traversal(lnode * huffman_bst, FILE * output_file, int * l_r, long int depth, long int dir) ;

int priority_creator(char * filename_output_huff, int *ascii_collect, char * filename_output_char) ;

lnode *huffman_tree(lnode * head) ;

lnode *huffman_helper(lnode * head) ;

int binary_creator(char * filename_input, char * filename_output, char * input_2, char * input_3) ;

int unhuff (char * file_input, char * file_output) ;

#endif
