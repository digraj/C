#ifndef __sorting_h_
#define __sorting_h_

#include <stdlib.h>
#include <stdio.h>

// Constant Definitions

/* Return/Error Codes */
#define OK               (  0 )  // No errors, everything as should be
#define ERROR            ( -1 ) // Generic error

// structures
  
/* Function Prototypes */
int Save_File(char *Filename, long *Array, int Size);
void Improved_Bubble_Sort(int *Array, int Size, double *N_Comp, double *N_Move);

#endif  // __sorting_h_ 

