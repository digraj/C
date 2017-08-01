#include <stdio.h>
#include <stdlib.h>
#include "sorting.h"

#define MAXFILELEN   (50)

void Save_Seq2 (char *Filename, int N)
{
	FILE * seq2_file = fopen(Filename, "w") ;	
	
	int n_preserve = N;
	int i = 0 ;
	int n_size = 10 ;
	float div = 1.3 ;

	long * sequence = (long *)calloc(n_size, sizeof(long)) ;
	if (sequence == NULL) {
		return ;
	}

	while ((i == 0) || (sequence[i - 1] > 1)) {
		sequence[i] = N / div ;
		div = div * 1.3 ;		
		if ((sequence[i] == 9) || (sequence[i] == 10)) {
			sequence[i] = 11 ;
		} 
		i++ ;
		if (i >= n_size) {
			n_size = n_size * 2 ;
			sequence = (long *)realloc(sequence, n_size * sizeof(long)) ;
			if (sequence == NULL) {
				return ;
			}
		}
	}	
	
	int j = 0 ;

	while (j < i) {
		fprintf(seq2_file, "%ld\n", sequence[j]) ;
		j++ ;
	}

	N = n_preserve ;

	free(sequence) ;
	fclose(seq2_file) ;

	return ;	
}

void Improved_Bubble_Sort (int *Array, int Size, double *NComp, double *NMove)
{
	char seq2[MAXFILELEN] = "seq2_file.txt";
	Save_Seq2 (seq2, Size) ;

	FILE * seq2_file = fopen(seq2, "r") ;

	fseek(seq2_file, 0, SEEK_SET) ;

	int i = 0 ;
	int cnt = 0 ;

	while ((cnt = fgetc(seq2_file)) != EOF) {
		if (cnt == '\n') {
			i++ ;
			cnt = fgetc(seq2_file) ;
		}
	}

	fseek(seq2_file, 0, SEEK_SET) ;

	long * gap = (long *)calloc(i + 1, sizeof(long)) ;
	i = 0 ;
	cnt = 0 ;
	long temp = 0 ;
	int l = 0 ;
	int j, k ;
	j = k = 0 ;
	
	while ((cnt = fgetc(seq2_file)) != EOF) {
		if (cnt == '\n') {
			i++ ;
			cnt = fgetc(seq2_file) ;
		}
		cnt = cnt - 48 ;
		gap[i] = (gap[i] * 10) + cnt ;
	}
	
	fclose(seq2_file) ;

	int gap_temp = 1 ;

	for (j = 0; j < i; j++) {
		gap_temp = gap[j] ;
		for (k = 0; k <= gap_temp; k++) {
			for (l = k; l < Size - gap_temp ; l = l + gap_temp) {
				if (Array[l] > Array[l + gap_temp]) {
					temp = Array[l] ;
					Array[l] = Array[l+gap_temp] ;
					Array[l+gap_temp] = temp ;
					*NMove = *NMove + 3 ;
				}					
				*NComp = *NComp + 1 ;
			}
		}		
	}
				 
	free(gap) ;

	FILE * output = fopen("Filename", "w") ;
	if (output == NULL) {
		return  ;
	}	

	i = 0 ;
	
	fprintf(output, "%d\n", Size) ;
	for (i = 0; i < Size; i++) {
		fprintf(output, "%d\n", Array[i]) ;
	}

	fclose(output) ;

}
				
