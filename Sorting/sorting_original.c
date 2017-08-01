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

	long * sequence = (long *)calloc(n_size, sizeof(long)) ;                // 2 * 3
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

void Save_Seq1 (char *Filename, int N)
{	
	FILE * seq1_file = fopen(Filename, "w") ;	
	
	int i, p, q, p_temp, q_temp ;
	i = p = q = p_temp = q_temp = 0 ;
	
	int n_size = 10 ;
										//  p	q				
	long * sequence = (long *)calloc(n_size, sizeof(long)) ;                // 2 * 3
	if (sequence == NULL) {
		return ;
	}

	sequence[0] = 1 ;
	sequence[1] = 2 ;
	sequence[2] = 3 ;
	sequence[3] = 4 ;
	sequence[4] = 6 ;

	i = 5 ;									//2 points at s[4], 3 points at s[3]
	int p2 = 3;
	int p3 = 2;
	while (((sequence[p2] * 2) < N) ||  ((sequence[p3] * 3) < N)) {
		if (sequence[p2] * 2 < sequence[p3] * 3) {
			sequence[i] = sequence[p2] * 2 ;
			p2++ ;
		}
		else if (sequence[p2] * 2 > sequence[p3] * 3) {
			sequence[i] = sequence[p3] * 3 ;
			p3++ ;
		}
		else if (sequence[p2] * 2 == sequence[p3] * 3) {   	
			sequence[i] = sequence[p2] * 2 ;
			p2++ ;
			p3++ ;
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
	
	while (i != 0) {
		fprintf(seq1_file, "%ld\n", sequence[i-1]) ;
		i-- ;
	}
			
	free(sequence) ;
	fclose(seq1_file) ;

	return ;	
}

void Shell_Insertion_Sort (long *Array, int Size, double *NComp, double *NMove)
{
		
	char seq1[MAXFILELEN] = "seq1_file.txt";
	Save_Seq1 (seq1, Size) ;	
	
	FILE * seq1_file = fopen(seq1, "r") ;

	long * gap = (long *)calloc(Size, sizeof(long)) ;
	int i = 0 ;
	int cnt = 0 ;
	int temp = 0 ;
	int l = 0 ;
	int j, k ;
	j = k = 0 ;
	
	while ((cnt = fgetc(seq1_file)) != EOF) {
		if (cnt == '\n') {
			i++ ;
			cnt = fgetc(seq1_file) ;
		}
		cnt = cnt - 48 ;
		gap[i] = (gap[i] * 10) + cnt ;
	}
	
	fclose(seq1_file) ;

	for (j = 0; j < i; j++) {
		for (k = gap[j]; k < Size; k++) {
			temp = Array[k] ;
			l = k ;
			*NMove = *NMove + 1 ;
			while ((l >= gap[j]) && (Array[l - gap[j]] > temp)) {
				Array[l] = Array[l - gap[j]] ;
				l = l - gap[j] ;
				*NMove = *NMove + 1 ;
				*NComp = *NComp + 1 ;
			}
			*NComp = *NComp + 1 ;
			*NMove = *NMove + 1 ;
			Array[l] = temp ;
		}
	}  
				 
	free(gap) ;

	return ;
}

void Improved_Bubble_Sort (long *Array, int Size, double *NComp, double *NMove)
{
	char seq2[MAXFILELEN] = "seq2_file.txt";
	Save_Seq2 (seq2, Size) ;

	FILE * seq2_file = fopen(seq2, "r") ;

	long * gap = (long *)calloc(Size, sizeof(long)) ;
	int i = 0 ;
	int cnt = 0 ;
	int temp = 0 ;
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
}

long *Load_File (char *Filename, int *Size)
{

	FILE * input = fopen(Filename, "r") ;
	if (input == NULL) {
		return NULL ;
	}

	fseek(input, 0, SEEK_SET) ;
	
	int cnt ;
	
	cnt = fgetc(input) ;
	cnt = cnt - 48 ;	
	*Size = cnt ;
	cnt = fgetc(input) ;

	while (cnt != '\n') {
		cnt = cnt - 48 ;
		*Size = (*Size * 10) + cnt ;
		cnt = fgetc(input) ;
	}		 

	long * Array = (long *)calloc(*Size + 1, sizeof(long)) ;

	int i = 0 ;

	while ((cnt = fgetc(input)) != EOF) {
		if (cnt == '\n') {
			i++ ;
			cnt = fgetc(input) ;
		}
		cnt = cnt - 48 ;
		Array[i] = (Array[i] * 10) + cnt ;
	}

	fclose(input) ;

	return Array ;
}

int Save_File (char *Filename, long *Array, int Size) 
{
	FILE * output = fopen(Filename, "w") ;
	if (output == NULL) {
		return EXIT_FAILURE ;
	}	

	int i = 0 ;
	
	fprintf(output, "%d\n", Size) ;
	for (i = 0; i < Size; i++) {
		fprintf(output, "%ld\n", Array[i]) ;
	}

	fclose(output) ;

	return i ;
}
/*


max = Size - 1 - gap_temp ;
		min = 0 ;
		for (k = max; k > min; k--) {
			for (l = min; l <= k; l = (l + gap_temp)) {
				if (Array[l] > Array[l+gap_temp]) {
					temp = Array[l] ;
					Array[l] = Array[l+gap_temp] ;
					Array[l+gap_temp] = temp ;
					*NMove = *NMove + 3 ;
				}
				*NComp = *NComp + 1;
			}
			min++ ;
			for (l = k; l >= (min + gap_temp); l = (l - gap_temp)) {
				if (Array[l] < Array[l-gap_temp]) {
					temp = Array[l] ;
					Array[l] = Array[l-gap_temp] ;
					Array[l-gap_temp] = temp ;
					*NMove = *NMove + 3 ;
				}
				*NComp = *NComp + 1;
			}
		}	





for (k = max; k >= 0; k--) {
			for (l = min; l <= k; l++) {
				if (Array[l] > Array[l+gap_temp]) {
					temp = Array[l] ;
					Array[l] = Array[l+gap_temp] ;
					Array[l+gap_temp] = temp ;
				}
			}
			min++ ;
			for (l = max; l >= min; l--) {
				if (Array[l] < Array[l-gap_temp]) {
					temp = Array[l] ;
					Array[l] = Array[l-gap_temp] ;
					Array[l-gap_temp] = temp ;
				}
			}
			k-- ;
		}




for (j = 0; j < i; j++) {
		fprintf(stderr, "gap : %ld, j = %d\n", gap[j], j) ;
		max = Size - 1 - gap[j] ;
		min = 0 ;		
		while (k > 0) {
			for (l = min ; l < k; l++) {
				if (Array[l] > Array[l + gap[j]]) {
					temp = Array[l] ;
					Array[l] = Array[l+gap[j]] ;
					Array[l+gap[j]] = temp ;
				}
		 	}
			for (l = k-1 ; l > (min + gap[j]); l--) {
				if (Array[l] < Array[l - gap[j]]) {
					temp = Array[l] ;
					Array[l] = Array[l-gap[j]] ;
					Array[l-gap[j]] = temp ;
				}
		 	}
		k-- ;
		min++ ;
		}
	}



max_j = Size - 1 - gap[j] ;
		while (detect >= 0) {
			detect = -1 ;
			for (k = 0; k <= max_j; k++) {
				if (Array[k] > Array[k + gap[j]]) {
					temp = Array[k] ;
					Array[k] = Array[k+gap[j]] ;
					Array[k+gap[j]] = temp ;
					detect = k ;
				}
			}
			k-- ;
*/
