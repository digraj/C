#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include "answer04.h"

int main(int argc, char ** argv) 
{
	if (argc != 5) {
		return EXIT_FAILURE ;
	}
	char * filename_input = argv[1] ;
	char * filename_output1 = argv[2] ;
	char * filename_output2 = argv[3] ;
	char * filename_output3 = argv[4] ;

	int ascii_collector[256] = {0};

	int rtn_val = File_256_lines(filename_input, ascii_collector) ;	
	if (rtn_val == -1) {
		return EXIT_FAILURE ;
	}

	rtn_val = priority_creator(filename_output1, ascii_collector, filename_output2) ;	
	if (rtn_val == -1) {
		return EXIT_FAILURE ;
	}	

	rtn_val = binary_creator(filename_output2, filename_output3) ;
	if (rtn_val == -1) {
		return EXIT_FAILURE ;
	}

	return EXIT_SUCCESS ;
}

