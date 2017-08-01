#include<stdio.h>
#include<stdlib.h> 
#include<string.h>
#include "bmp.h"

int main(int argc, char ** argv) 
{	
	BMP_Image * t_image = NULL ;
	int i = 0 ;
	if (argc == 3) {
		if (i == 0) {
			if ((argv[0][0] != '.') || (argv[0][1] != '/')|| (argv[0][2] != 'p') || (argv[0][3] != 'a') || (argv[0][4] != '0') || (argv[0][5] != '5') || (argv[0][6] != '\0')) {
				fprintf(stderr, "Invalid first argument. Wrong Executable file.\n") ;						
				return EXIT_FAILURE ;
			}
		}
		
		char *filename_input = argv[1] ;
		FILE * input_file = fopen(filename_input, "r") ;
		if (input_file == NULL) {
			fprintf(stderr, "Input file not found / could not be opened.\n") ;		
			return EXIT_FAILURE ;	
		}	
		int cnt ;
		if ((cnt = fgetc(input_file)) == EOF) {	
			fclose(input_file) ;
			fprintf(stderr, "Input file empty.\n") ;	
			return EXIT_FAILURE ;
		}
		BMP_Image * image = Read_BMP_Image(input_file) ; 		
		if (image == NULL) {
			fprintf(stderr, "Image could not be allocated\n") ;
			fclose(input_file) ;					
			return EXIT_FAILURE ;
		}
		char *filename_output = argv[2] ;
		FILE * output_file = fopen(filename_output, "w") ;	
		if (output_file == NULL) {
			Free_BMP_Image(image) ;
			fclose(input_file) ;
			fprintf(stderr, "Output file not be created.\n") ;				
			return EXIT_FAILURE ;	
		}	
		fclose(input_file) ;				
		if (image->header.bits == 24) {
			t_image = Convert_24_to_16_BMP_Image_with_Dithering(image) ;
		}
		else if (image->header.bits == 16) {
			t_image = Convert_16_to_24_BMP_Image(image) ;
		}
		Write_BMP_Image(output_file, t_image) ;
		Free_BMP_Image(image) ;
		Free_BMP_Image(t_image) ;
		fclose(output_file) ;
	}
	else {
		fprintf(stderr, "Not Enough Inputs / Extra Inputs.\n") ;		
		return EXIT_FAILURE ;	
	}
	return EXIT_SUCCESS ;
}
