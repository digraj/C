#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "answer01.h"

double my_function(double) ;
 
int main(int argc, char *argv[])
{
	double integral ;
	fourier fourier_arg; 
	
	fourier_arg.intg.lower_limit = atof(argv[2]) ;
        fourier_arg.intg.upper_limit = atof(argv[3]) ;
	fourier_arg.intg.n_intervals = atoi(argv[4]) ;
	fourier_arg.n_terms = atoi(argv[5]) ;
	int n = fourier_arg.intg.n_intervals ;
	
        fourier_arg.intg.func_to_be_integrated = my_function ;	
	
	integral = simpson_numerical_integration_for_fourier(fourier_arg.intg, n, cos) ;	

	printf("%f %f\n", *fourier_arg.a_i, *fourier_arg.b_i) ;
	return EXIT_SUCCESS ;
}

double my_function(double x) 
{
	int integral = x ; 
	return integral ;
} 
