#include <stdlib.h>
#include <stdio.h>

#include "bmp.h"

///////////////////////CHECK FOR RETURN STATEMENTS DURING FAILURE TO ALLOC AND OTHER STUFF) ///////////////////////

/* check whether a header is valid
 * assume that header has been read from fptr
 * the position of the indicator of fptr is not certain
 * could be at the beginning of the file, end of the file or 
 * anywhere in the file
 * note that the check is only for this exercise/assignment
 * in general, the format is more complicated
 */

int Is_BMP_Header_Valid(BMP_Header* header, FILE *fptr) 
{
	fseek(fptr, 0, SEEK_SET) ;
  // Make sure this is a BMP file
  	if (header->type != 0x4d42) {
     		return FALSE;
  	}
  // skip the two unused reserved fields

  // check the offset from beginning of file to image data
  // essentially the size of the BMP header
  // BMP_HEADER_SIZE for this exercise/assignment
  	if (header->offset != BMP_HEADER_SIZE) {
     		return FALSE;
  	}
      
  // check the DIB header size == DIB_HEADER_SIZE
  // For this exercise/assignment
  	if (header->DIB_header_size != DIB_HEADER_SIZE) {
     		return FALSE;
  	}

  // Make sure there is only one image plane
  	if (header->planes != 1) {
    		return FALSE;
  	}
  // Make sure there is no compression
  	if (header->compression != 0) {
    		return FALSE;
  	}

  // skip the test for xresolution, yresolution

  // ncolours and importantcolours should be 0
  	if (header->ncolours != 0) {
    		return FALSE;
  	}
  	if (header->importantcolours != 0) {
    		return FALSE;
  	}
  
  // Make sure we are getting 24 bits per pixel
  // or 16 bits per pixel
  // only for this assignment
	if (header->bits != 24 && header->bits != 16) {
	    	return FALSE;
	}

  // fill in extra to check for file size, image size
  // based on bits, width, and height

	if (header->width <= 0) {
		return FALSE ;
	}
	
	if (header->height <= 0) {
		return FALSE ;
	}
		
	if (header->bits <= 0) {
		return FALSE ;
	}
	if (header->size != (header->imagesize + 54)) {
		return FALSE ;
	}

	int diff = (header->width) % 4 ;
	int bits = (header->bits) / 8 ;

	if (bits == 2) {
		diff = 0 ;
	}

	if ((bits == 2) && ((((header->width * 2) * header->height) % 4) != 0)) {
		diff = 2 ;
	}

	int check = (((header->width * bits) + diff) * header->height) + 54 ;
	
	if (check != header->size) {
		return FALSE ;
	}

  	return TRUE;
}

/* The input argument is the source file pointer. 
 * The function returns an address to a dynamically allocated BMP_Image only 
 * if the file * contains a valid image file 
 * Otherwise, return NULL
 * If the function cannot get the necessary memory to store the image, also 
 * return NULL
 * Any error messages should be printed to stderr
 */
BMP_Image *Read_BMP_Image(FILE* fptr) 
{

  // go to the beginning of the file

	fseek(fptr, 0, SEEK_END) ;
	int file_size = ftell(fptr) ;
   	BMP_Image *bmp_image = NULL;

  //Allocate memory for BMP_Image*;

	bmp_image = (BMP_Image *)malloc(file_size) ;
	if (bmp_image == NULL) { 
		return NULL ;
	}

  //Read the first 54 bytes of the source into the header

	fseek(fptr, 0, SEEK_SET) ;
	fread(&(bmp_image->header), sizeof(BMP_Header), 1, fptr) ;

  // if read successful, check validity of header

	int rtn_val = Is_BMP_Header_Valid(&(bmp_image->header), fptr) ;	
	if (rtn_val != TRUE ) {
		free(bmp_image) ;
		return NULL ;
	}
  // Allocate memory for image data

	bmp_image->data = (unsigned char *)malloc(sizeof(unsigned char) * (file_size - sizeof(BMP_Header))) ;
	if (bmp_image->data == NULL) {
		Free_BMP_Image(bmp_image) ; 
		return NULL ;
	}
  // read in the image data

	fseek(fptr, sizeof(BMP_Header), SEEK_SET) ;
	fread(bmp_image->data, file_size - sizeof(BMP_Header), 1, fptr) ;

  	return bmp_image;
}

/* The input arguments are the destination file pointer, BMP_Image *image.
 * The function write the header and image data into the destination file.
 * return TRUE if write is successful
 * FALSE otherwise
 */
int Write_BMP_Image(FILE* fptr, BMP_Image* image) 
{
   // go to the beginning of the file

	fseek(fptr, 0, SEEK_SET) ;

   // write header

	fwrite(&(image->header), sizeof(BMP_Header), 1, fptr) ;

   // write image data
	fseek(fptr, sizeof(BMP_Header), SEEK_SET) ;
	fwrite(image->data, image->header.imagesize, 1, fptr) ;

   return TRUE;
}

/* The input argument is the BMP_Image pointer. The function frees memory of 
 * the BMP_Image.
 */
void Free_BMP_Image(BMP_Image* image) 
{
	free(image->data) ;
	free(image) ;
}

void print_char_in_bits(char number)
{
   int i;
   unsigned char mask = 1 << (8 - 1);
   for (i = 0; i < 8; i++) {
      int result = mask & number;
      if (result == 0) {
         printf("0");
      } else {
         printf("1");
      }
      mask >>= 1;
   }
}

BMP_Image *Convert_24_to_16_BMP_Image_with_Dithering(BMP_Image *image)
{
   	BMP_Image *t_image = NULL;
	int height = image->header.height ;
	int width = image->header.width ;
	int bits_pi = image->header.bits ;

	if (bits_pi == 24) {
		bits_pi = 3 ;
	}
	else if (bits_pi == 16) {
		bits_pi = 2 ;
	}	

	int i, j, k ;
	i = j = k = 0 ;

	int diff = (image->header.width) % 4 ;
	int diff_2 = diff ;
	if (diff % 2 != 0) {
		diff = 2 ;
	}
	else {
		diff = 0 ;
	}

	int ***array_image ;
	
	array_image = (int ***)malloc(sizeof(int **)*height);
   	if (array_image == NULL) {
      		return NULL ;
   	}
   	for (i = 0; i < height; i++) {
      		array_image[i] = (int **)malloc(sizeof(int *)*width);
      		if (array_image[i] == NULL) {
         	int l;
         	for (k = 0; k < i; k++) {
            		for (l = 0; l < width; l++) {
               			free(array_image[k][l]);
            		}
         	   	free(array_image[k]);
         	}
         	free(array_image);
         	return NULL;
      		}
     		for (j = 0; j < width; j++) {
		 	array_image[i][j] = (int *)malloc(sizeof(int)*3);
		 	if (array_image[i][j] == NULL) {
		    	int l;
		    	for (l = 0; l < j; l++) {
		       		free(array_image[i][l]);
		    	}
		    	for (k = 0; k < i; k++) {
		       		for (l = 0; l < width; l++) {
		          		free(array_image[k][l]);
		       		}	
		       		free(array_image[k]);
		    	}
		    	free(array_image);
		    	return NULL;
		 	}
      		}
   	}

	t_image = (BMP_Image *)malloc(image->header.size) ;
	t_image->data = (unsigned char *)malloc(height * ((width * 2) + diff)) ;
	t_image->header = image->header ;
	t_image->header.bits = 16 ;
	t_image->header.size = (height * ((width * 2) + diff)) + 54 ;
	t_image->header.imagesize = (height * ((width * 2) + diff)) ;

	int counter = 0 ;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			for (k = 0; k < bits_pi; k++) {
				array_image[height - i - 1][j][k] = image->data[counter] ;
				counter++ ;
			}
		}
		counter = counter + diff_2 ;
	}


	int temp = 0x00;
	unsigned char preserve = 0x00;
	unsigned char put_1 = 0x00;
	unsigned char put_2 = 0x00;
	unsigned char array_image_2[height][width][2] ;
	int quant_error = 0 ; 
	char quant_error_to_add[height][width][3] ; 
	int added = 0 ;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			for (k = 0; k < 3; k++) {
				quant_error_to_add[i][j][k] = 0 ;
			}
		}
	}

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			for (k = 0; k < 3; k++) {
				quant_error_to_add[i][j][k] /= 16 ;
				added = array_image[i][j][k] + quant_error_to_add[i][j][k];
				if (added > 255) {
					added = 255 ;
				}
				if (added < 0) {
					added = 0 ;
				}
				array_image[i][j][k] = (int)added;	
				temp = 	array_image[i][j][k] ;
				temp = temp >> 3 ;	
				quant_error = array_image[i][j][k] - ((temp * 255) / 31) ;
				if ((i != (height - 1)) & (j != 0)) {
					quant_error_to_add[i + 1][j - 1][k] = quant_error_to_add[i + 1][j - 1][k] + (quant_error * 3) ; 			
				}
				if (i != (height - 1)) {
					quant_error_to_add[i + 1][j][k] = quant_error_to_add[i + 1][j][k] + (quant_error * 5) ;
				}
				if ((i != (height - 1)) && (j != (width - 1))) {  
					quant_error_to_add[i + 1][j + 1][k] = quant_error_to_add[i + 1][j + 1][k] + (quant_error * 1);
				}
				if (j != (width - 1)) {  
					quant_error_to_add[i][j + 1][k] = quant_error_to_add[i][j + 1][k] + (quant_error * 7) ;				
				}				
				
					
			} 	
		}
	}

	i = j = k = 0 ;
	temp = 0x00 ;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			for (k = 0; k < 3; k++) {
				temp = 	array_image[i][j][k] ;
				temp = temp >> 3 ;			
				if (k == 0) {
					put_1 = temp | put_1 ;
				}
				if (k == 1) {
					preserve = temp >> 3 ;
					temp = temp << 5 ;
					put_1 = temp | put_1 ;
				}
				if (k == 2) {
					temp = temp << 2 ;
					temp = temp | preserve ;
					put_2 = temp ;
				}
				temp = 0x00 ;									
			} 
			array_image_2[i][j][0] = put_1 ; 
			array_image_2[i][j][1] = put_2 ;
			temp = 0x00 ;
			put_1 = 0x00;
	                put_2 = 0x00;	
			preserve = 0x00 ;	
		}
	}

	counter = 0 ;

	for (i = 0; i < height; i++) {
		for (j = 0; j < ((2 * width) + diff); j++) {
			t_image->data[counter] = 0x00 ;
			counter++ ;
		}
	}

	counter = 0 ;
	i = j = k = 0;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			for (k = 0; k < 2; k++) {
				t_image->data[counter] = array_image_2[height - i -1][j][k] ;
				counter++ ;
			}
		}
		counter = counter + diff ;
	}

	
	for (i = 0; i < height; i++) {
      		for (j = 0; j < width; j++) {
         		free(array_image[i][j]);
      		}
      		free(array_image[i]);
   	}
   	free(array_image);


	return t_image ;
}


BMP_Image *Convert_16_to_24_BMP_Image(BMP_Image *image)
{
   	BMP_Image *t_image = NULL;
	int height = image->header.height ;
	int width = image->header.width ;
	int bits_pi = image->header.bits ;

	if (bits_pi == 24) {
		bits_pi = 3 ;
	}
	else if (bits_pi == 16) {
		bits_pi = 2 ;
	}	

	int diff = image->header.width % 4 ;

	int diff_2 = 0 ;
	
	if ((bits_pi == 2) && ((((width * 2) * height) % 4) != 0)) {
		diff_2 = 2 ;
	}

	int i, j, k ;
	i = j = k = 0 ;

	unsigned char array_image[height][width][2] ;

	t_image = (BMP_Image *)malloc(image->header.size) ;
	t_image->data = (unsigned char *)malloc(height * ((width * 3) + diff)) ;
	t_image->header = image->header ;
	t_image->header.bits = 24 ;
	t_image->header.size = (height * ((width * 3) + diff)) + 54;
	t_image->header.imagesize = height * ((width * 3) + diff) ;

	int counter = 0 ;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			for (k = 0; k < bits_pi; k++) {
				array_image[i][j][k] = image->data[counter] ;				
				counter++ ;
			}
		}
		counter = counter + diff_2 ;
	}

	unsigned char temp = 0x00;
	unsigned char preserve = 0x00;
	int put_1 = 0x00;
	int put_2 = 0x00;
	int put_3 = 0x00;
	unsigned char get_1 = 0x00;
	unsigned char get_2 = 0x00;
	unsigned char array_image_2[height][width][3] ;

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			for (k = 0; k < 2; k++) {
				if (k == 0) {
					get_1 = array_image[i][j][k] ;	
				}
				if (k == 1) {
					get_2 = array_image[i][j][k] ;
				}
			} 
			preserve = get_1 >> 5 ;
			get_1 = get_1 << 3 ;
			get_1 = get_1 >> 3 ;
			put_1 = (get_1 * 255) / 31 ; 
			temp = get_2 >> 2 ;
			get_2 = get_2 << 6 ;
			get_2 = get_2 >> 3 ;
			put_2 = (get_2 | preserve) ;
			put_2 = (put_2 * 255) / 31 ;
			put_3 = (temp * 255) / 31 ;			
			array_image_2[i][j][0] = (unsigned char)put_1;
			array_image_2[i][j][1] = (unsigned char)put_2;
			array_image_2[i][j][2] = (unsigned char)put_3;
			temp = 0x00;
			preserve = 0x00;
			put_1 = 0x00;
			put_2 = 0x00;
			put_3 = 0x00;
			get_1 = 0x00;
			get_2 = 0x00;
		}
	}

	counter = 0 ;

	for (i = 0; i < height; i++) {
		for (j = 0; j < ((3 * width) + diff); j++) {
			t_image->data[counter] = 0x00 ;
			counter++ ;
		}
	}

	counter = 0 ;
	i = j = k = 0;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			for (k = 0; k < 3; k++) {
				t_image->data[counter] = array_image_2[i][j][k] ;
				counter++ ;
			}
		}
		counter = counter + diff ;
	}

	return t_image ;
}











