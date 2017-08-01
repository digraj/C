#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include "answer04.h"

int main(int argc, char ** argv) 
{
	char * filename_input = argv[1] ;
	char * filename_output1 = "file1" ;
	char * filename_output2 = "file2" ;
	char * filename_output3 = "file3" ;
	FILE * temp = fopen("filea", "w") ;
	fprintf(temp, "%s", argv[1]) ;
	char * filename_output4 = argv[2] ;

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
//	printf("hi") ;
	unhuff(filename_output3, filename_output4) ;
//	printf("nope") ;
	return EXIT_SUCCESS ;
}
