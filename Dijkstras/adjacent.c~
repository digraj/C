#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct _lnode {
	struct _lnode * next;
	int value ;
	int distance ;
} lnode ;

int findmin(int * vertices_weight, int vertices, int * visited) 
{
	int i = 0 ;
	int min = 0 ;
	int min_weight = 0 ;

	for (i = 0; i < vertices; i++) {
		if (visited[i] != 1) {
			if (vertices_weight[i] > -1) {
				min_weight = vertices_weight[i] ;
				break ;
			}
		}
	}
	int j = 0 ;
	for (j = i - 1; j < vertices; j++) {
		if (j > -1) {
			if (visited[j] != 1) {
				if ((vertices_weight[j] > -1) && (vertices_weight[j] <= min_weight)) {
					min_weight = vertices_weight[j] ;
					min = j ;
				}
			}
		}
	}

	return min ;
}

int create_path(int * previous, int source, int destination, int * path) 
{
	path[0] = destination ;
	int i = 1 ;
	int current = destination ;

	while (current != source) {	
		current = previous[current] ;
		path[i] = current ;
		i++ ;
	}	

	return i ;
}

void print_path(int * path, int i) 
{
	int k = 0 ;

	for (k = i - 1; k >= 0; k--) {
		printf("%d ", path[k]);
	}

	printf("\n") ;
}	

int shortest_distance(char * filename, lnode ** list, int vertices, int edges) 
{
	FILE * input = fopen(filename, "r") ;
	if (input == NULL) {
		return -1 ;
	}

	int cnt = 0 ;
	int queries = 0 ;

	while ((cnt = fgetc(input)) != '\n') {
		queries *= 10 ;
		queries = queries + (cnt - 48) ;
	}

	int * coordinates = (int *) calloc (2 * queries, sizeof(int)) ;
	int num = 0 ;
	int i = 0 ;
	
	while ((cnt = fgetc(input)) != EOF) {
		fseek(input, -1, SEEK_CUR) ;
		
		while ((cnt = fgetc(input)) == 32) {
		}

		fseek(input, -1, SEEK_CUR) ;

		while ((cnt = fgetc(input)) != 32) {
			num *= 10 ;
			num = num + (cnt - 48) ;
		}

		coordinates[i++] = num ;
		num = 0 ;
	
		while ((cnt = fgetc(input)) == 32) {
		}

		fseek(input, -1, SEEK_CUR) ;

		while ((cnt = fgetc(input)) != '\n') {
			num *= 10 ;
			num = num + (cnt - 48) ;
		}
		coordinates[i++] = num ;
		num = 0 ;
	}	

	i = 0 ;
	int j = 0 ;
	int m = 0;

	lnode * head = NULL ;

	int weight = 0;
	int * vertices_weight = (int *) calloc (vertices, sizeof(int)) ;
	int * visited = (int *) calloc (vertices, sizeof(int)) ;
	int * previous = (int *) calloc (vertices, sizeof(int)) ;
	int * path = (int *) calloc (vertices, sizeof(int)) ;	
	int k = 0 ;

	while (m < (2 * queries)) {
		j = coordinates[m] ;	
		m++ ;
		for (i = 0; i < vertices; i++) {
			visited[i] = 0 ;
			vertices_weight[i] = -1 ;
		}
		vertices_weight[j] = 0 ;

		i = 0 ;

		while (i < vertices) {
			if (j == coordinates[m]) {
				break ;
			}
			head = list[j] ;
			visited[j] = 1 ;
			while (head->value != -1) {
				if (visited[head->value] != 1) {
					weight = vertices_weight[j] + head->distance ;
					if ((vertices_weight[head->value] == -1) || (weight <= vertices_weight[head->value]))
						vertices_weight[head->value] = weight ;
						previous[head->value] = j ;			
				}			
				head = head->next ;
			}
			j = findmin(vertices_weight, vertices, visited) ;
			visited[j] = 1 ;
			i++ ;				
		}
		m++ ;
		if (vertices_weight[coordinates[m - 1]] == -1) {
			printf("INF\n") ;
		}
		else {
			printf("%d\n", vertices_weight[coordinates[m - 1]]) ;
		}
		k = create_path(previous, coordinates[m - 2], coordinates[m - 1], path) ;
		print_path(path, k) ;
	}
	
	fclose(input) ;
	free(coordinates) ;
	free(vertices_weight) ;
	free(visited) ;
	free(previous) ;
	free(path) ;

	return 0 ;		
	
}		


int adjacent_list (char * filename, char * filename_2) 
{
	FILE * input = fopen(filename, "r") ;
	if (input == NULL) {
		return -1 ;
	}
//	FILE * out = fopen("out", "w") ;
//	if (out == NULL) {
//		return -1 ;
//	}

	fseek(input, 0, SEEK_SET) ;

	int cnt ;
	int vertices = 0;
	int edges = 0 ;

	while ((cnt = fgetc(input)) != 32) {
		vertices *= 10 ;
		vertices = vertices + (cnt - 48) ;
	}

	fseek(input, -1, SEEK_CUR) ;

	while ((cnt = fgetc(input)) == 32) {
	}

	fseek(input, -1, SEEK_CUR) ;

	while ((cnt = fgetc(input)) != '\n') {
		edges *= 10 ;
		edges = edges + (cnt - 48) ;
	}

	fseek(input, 0, SEEK_SET) ;	

	int line_cnt = 0;
	int num = 0 ;
	int val = 0 ;
	while ((cnt = fgetc(input)) != '\n') {
	}

	int * coordinates = (int *) calloc (3 * vertices, sizeof(int)) ;

	while (line_cnt != vertices) {
		if ((cnt = fgetc(input)) == 32) {
			fseek(input, -1, SEEK_CUR) ;	
			while ((cnt = fgetc(input)) == 32) {
			}
		}
		
		fseek(input, -1, SEEK_CUR) ;

		while ((cnt = fgetc(input)) != 32) {
			num *= 10 ;
			num += cnt - 48 ;
		}
		
		coordinates[num * 3] = num ;
		
		while ((cnt = fgetc(input)) == 32) {
		}
		
		fseek(input, -1, SEEK_CUR) ;

		while ((cnt = fgetc(input)) != 32) {
			val *= 10 ;
			val += cnt - 48 ;
		}
		
		coordinates[num * 3 + 1] = val ;
		val = 0 ;

		while ((cnt = fgetc(input)) == 32) {
		}
		
		fseek(input, -1, SEEK_CUR) ;

		while ((cnt = fgetc(input)) != '\n') {
			val *= 10 ;
			val += cnt - 48 ;
		}
		coordinates[num * 3 + 2] = val ;
		val = 0 ;
		num = 0 ;
		line_cnt++ ;
	}
	
	fseek(input, 0, SEEK_SET) ;
	line_cnt = 0 ;

	while (line_cnt != vertices + 1) {
		cnt = fgetc(input) ;
		if (cnt == '\n') {
			line_cnt++ ;
		}
	}

	num = 0 ;
	val = 0 ;

	lnode ** list = (lnode **) calloc (vertices, sizeof(lnode *)) ;

	int * series_1 = (int *) calloc (edges, sizeof(int)) ;
	int * series_2 = (int *) calloc (edges, sizeof(int)) ;
	int * numbers = (int *) calloc (vertices , sizeof(int)) ;
	lnode * head = NULL ;	
	lnode * cur = NULL;
	int dist = 0 ;
	int i = 0;

	while ((cnt = fgetc(input)) != EOF) {
		fseek(input, -1, SEEK_CUR) ;		
		if ((cnt = fgetc(input)) == 32) {
			fseek(input, -1, SEEK_CUR) ;	
			while ((cnt = fgetc(input)) == 32) {
			}
		}
		
		fseek(input, -1, SEEK_CUR) ;
		
		while ((cnt = fgetc(input)) != 32) {
			num *= 10 ;
			num += cnt - 48 ;
		}
		series_1[i] = num ;
		while ((cnt = fgetc(input)) == 32) {
		}

		fseek(input, -1, SEEK_CUR) ;

		while ((cnt = fgetc(input)) != '\n') {
			val *= 10 ;
			val += cnt - 48 ;
		}
		
		series_2[i] = val ;
		num = 0 ;	
		val = 0 ;
		i++ ;	
	}
	i = 0 ;
	num = 0 ;
	val = 0 ;
	while (i < edges) {
		num = series_1[i] ;

		if (list[num] == NULL) { 		
			list[num] = (lnode *) calloc (1, sizeof(lnode)) ;	
		}		

		numbers[num] = 1 ;		
		head = list[num] ;
		cur = head ;
		while (head != NULL) {
			cur = head ;
			head = head->next ;		  	
		}		
		
		head = cur ;
		val = series_2[i] ;
		
		head->value = val ;
		dist = floor(sqrt(pow(coordinates[val * 3 + 1] - coordinates[num * 3 + 1], 2) + pow(coordinates[val * 3 + 2] - coordinates[num * 3 + 2], 2))) ;
		head->distance = dist ;			
		cur = (lnode *) calloc (1, sizeof(lnode)) ;
		head->next = cur ;
		head->next->value = -1 ;		
		head->next->next = NULL ;
		num = 0 ;	
		val = 0 ;
		i++ ;	
	}	
	i = 0 ;
	num = 0 ;
	val = 0 ;
	while (i < edges) {
		num = series_2[i] ;

		if (list[num] == NULL) { 		
			list[num] = (lnode *) calloc (1, sizeof(lnode)) ;	
		}		

		numbers[num] = 1 ;		
		head = list[num] ;
		cur = head ;
		while (head != NULL) {
			cur = head ;
			head = head->next ;		  	
		}		
		
		head = cur ;
		val = series_1[i] ;
		
		head->value = val ;
		dist = sqrt(pow(coordinates[val * 3 + 1] - coordinates[num * 3 + 1], 2) + pow(coordinates[val * 3 + 2] - coordinates[num * 3 + 2], 2)) ;
		head->distance = dist ;			
		cur = (lnode *) calloc (1, sizeof(lnode)) ;
		head->next = cur ;
		head->next->value = -1 ;		
		head->next->next = NULL ;
		num = 0 ;	
		val = 0 ;
		i++ ;	
	}

	shortest_distance(filename_2, list, vertices, edges) ;	
/*	for (i = 0 ; i < vertices; i++) {
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
*/	//free memory	
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
	
	free(series_1) ;
	free(series_2) ;
	free(list) ;
	free(numbers) ;
	free(coordinates) ;

	return 0 ;
}

int main (int argc, char ** argv) 
{
	char * filename_1 = argv[1] ;
	char * filename_2 = argv[2] ;
	int val = adjacent_list(filename_1, filename_2) ;
	
//	fprintf(stderr, "\nBack\n") ;

	if (val == -1) {
		fprintf(stderr, "\nError\n") ;
		return EXIT_FAILURE ;
	}

	return EXIT_SUCCESS ;
}



/*	
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


	for (i = 0; i < (3 * vertices); i++) {
		if (i % 3 == 0) printf("\n") ;
		fprintf(stderr, "%d ", coordinates[i]) ;
	}
*/

/*
	while ((cnt = fgetc(input)) != EOF) {
		fseek(input, -1, SEEK_CUR) ;		
		if ((cnt = fgetc(input)) == 32) {
			fseek(input, -1, SEEK_CUR) ;	
			while ((cnt = fgetc(input)) == 32) {
			}
		}
		
		fseek(input, -1, SEEK_CUR) ;
		
		while ((cnt = fgetc(input)) != 32) {
			num *= 10 ;
			num += cnt - 48 ;
		}
		
		while ((cnt = fgetc(input)) == 32) {
		}

		if (list[num] == NULL) { 		
			list[num] = (lnode *) calloc (1, sizeof(lnode)) ;	
		}		

		numbers[num] = 1 ;		
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
		dist = sqrt(pow(coordinates[val * 3 + 1] - coordinates[num * 3 + 1], 2) + pow(coordinates[val * 3 + 2] - coordinates[num * 3 + 2], 2)) ;
		head->distance = dist ;			
		cur = (lnode *) calloc (1, sizeof(lnode)) ;
		head->next = cur ;
		head->next->value = -1 ;		
		head->next->next = NULL ;
		num = 0 ;	
		val = 0 ;	
	}
*/
