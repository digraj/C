#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "answer03.h"

typedef struct _distance { 
	int ** coordinate ;
} distance;

Maze *Allocate_maze_space(int nrow, int ncol)
{
	Maze *maze ;
	maze = (Maze *)malloc(sizeof(Maze)) ;
	if (maze == NULL) {
		return NULL ;
	}
	int i = 0 ;
	int j = 0 ;
	maze->maze_array = (char **)malloc(sizeof(char *) * nrow) ;
	if ((maze->maze_array == NULL)) {
		free(maze) ;
		return NULL ;
	}
	for (; i < nrow; i++) {
		maze->maze_array[i] = (char *)malloc(sizeof(char) * ncol) ;
		if ((maze->maze_array[i] == NULL)) {
			for (j = 0; j < i; j++) {
				free(maze->maze_array[j]) ;
			}
			free(maze->maze_array) ;
			free(maze) ;
			return NULL ;
		} 	
	}
	maze->nrow = nrow ;	
	maze->ncol = ncol ;
	return maze;
}

// free the memory used for the maze
// you may assume that maze is not NULL, and all memory addresses are valid

void Deallocate_maze_space(Maze *maze)
{
	int i = 0 ;
	for (; i < maze->nrow; i++) {
		free(maze->maze_array[i]) ;
	}
	free(maze->maze_array) ;
	free(maze) ;
   return;
}

Maze *Read_maze_from_2Dfile(FILE *fptr)
{
	int i = 0 ;
	int j = 0 ;
	int rows = 0 ;
	int cols = 0 ;
	fseek(fptr, 0, SEEK_SET) ;
	int cnt ;
	int limiter = 0 ;
	while ((cnt = fgetc(fptr)) != EOF) {
		if (cnt == '\n') {
			rows = rows + 1 ;
			limiter = 1 ;
		}
		if (limiter == 0) {
			cols = cols + 1 ;
		}
	}

	Maze *maze = Allocate_maze_space(rows, cols) ;
	fseek(fptr, 0, SEEK_SET) ;
	for (i = 0; i < rows; i++) {
		for (j = 0; j <= cols; j++) {
			if ((cnt = fgetc(fptr)) != 10) {
				fseek(fptr, -1, SEEK_CUR) ;
				if ((cnt = fgetc(fptr)) != EOF) {
					maze->maze_array[i][j] = cnt ;
				}
			}
		}
	}

   	return maze;
}

int Write_maze_to_2Dfile(char *filename, const Maze *maze)
{
	FILE *put_maze ;
	int i = 0;
	int j = 0 ;	
	int counter = 0 ;	
	put_maze = fopen(filename, "w") ;
	if (put_maze == NULL) {
		return EXIT_FAILURE ;
	}
	//fseek(put_maze, 0, SEEK_SET) ;
	for (i = 0; i < (maze->nrow); i++) {
		for(j = 0; j < (maze->ncol); j++) {
			fputc(maze->maze_array[i][j], put_maze) ;
			counter++ ;
		}
		fputc('\n', put_maze) ;
		counter++ ;
	}
	
	fclose(put_maze) ;
   	return counter ;
}

/* given a maze, given a location cur, is that PATH */
/* if out of bound, return 0                        */
/* if within bound, but not PATH, return 0          */
/* otherwise return 1                               */

static int Is_path(Maze *maze, int row, int col)
{
   if ((row >= 0) && (row < maze->nrow)) {
      if ((col >= 0) && (col < maze->ncol)) {
         return maze->maze_array[row][col] == PATH;
      }
   }
   return 0;
}

/* given a maze, current row (curr),                                     */
/* current column (curc), destination row (drow), destination col (dcol) */
/* current distance from source, determine the where to explore from the */
/* current location                                                      */
/* a recursive function: from the current location, try 'N', 'S', 'E',   */
/* 'W' locations one at a time if the attempt is successful, write the   */
/* direction in the file                                                 */
/*
static int Pathfinder_helper(Maze *maze, int curr, int curc, int endr, int endc,
                            int count, FILE *dfptr)
{
   // you may uncomment this and use the output here to trace the function
   // with sample5.pdf
   printf("(%d, %d), %d\n", curr, curc, count);
   maze->maze_array[curr][curc] = VISITED;
   if ((curr == endr) && (curc == endc)) { // reach the destination 
      int i;
      for (i = 0; i < count; i++) { // create enough space in file
         fputc(VISITED, dfptr);
      }
      return count;
   }

   int found;
   if (Is_path(maze, curr-1, curc)) {
      found = Pathfinder_helper(maze, curr-1, curc, endr, endc, count+1, dfptr);
      if (found != -1) {
         fseek(dfptr, -1, SEEK_CUR); // go back one position
         fputc('N', dfptr);
         fseek(dfptr, -1, SEEK_CUR); // go back one position
         return found;
      }
   }
   if (Is_path(maze, curr+1, curc)) {
      found = Pathfinder_helper(maze, curr+1, curc, endr, endc, count+1, dfptr);
      if (found != -1) {
         fseek(dfptr, -1, SEEK_CUR); // go back one position
         fputc('S', dfptr);
         fseek(dfptr, -1, SEEK_CUR); // go back one position
         return found;
      }
   }
   if (Is_path(maze, curr, curc+1)) {
      found = Pathfinder_helper(maze, curr, curc+1, endr, endc, count+1, dfptr);
      if (found != -1) {
         fseek(dfptr, -1, SEEK_CUR); // go back one position
         fputc('E', dfptr);
         fseek(dfptr, -1, SEEK_CUR); // go back one position
         return found;
      }
   }
   if (Is_path(maze, curr, curc-1)) {
      found = Pathfinder_helper(maze, curr, curc-1, endr, endc, count+1, dfptr);
      if (found != -1) {
         fseek(dfptr, -1, SEEK_CUR); // go back one position
         fputc('W', dfptr);
         fseek(dfptr, -1, SEEK_CUR); // go back one position
         return found;
      }
   }
	maze->maze_array[curr][curc] = PATH ;
   return -1;
}

/* this is a function implemented by the instructor to illustrate           */
/* how to use a recursive function to go from the left-most top entrance to */
/* the right-most bottom exit of a maze.  The function will print the       */
/* directions taken to reach the bottom exit, without visiting a location   */
/* twice.  Note that the recursion may visit a location a few times         */
/* Given three filenames, first one is a given maze, the second one is to   */
/* be written with the directions, the third is the explored maze           */
/* If the maze file does not exist, the function immediately return -1 and  */
/* do nothing else                                                          */
/* the directions are in the form of a series of characters                 */
/* 'N', 'S', 'E', 'W', without spacing (and without the quotes).            */
/* There should be just a single line, without a terminating newline        */
/* character. In other words, to the vi editor, it is an incomplete file    */
/* you are to assume that you start from the top-most, left-most entry      */
/* of the maze.  Therefore, the first direction is always 'S'               */
/* similarly, the last direction is always 'S'                              */
/* the returned value should be the number of steps taken to reach          */
/* the right-most bottom exit.  It is typically not the shortest distance   */
/* Note that the maze can be generated by amaze, or some combinations of    */
/* the maze expansion functions that you have written in PE08               */
/* you may assume that the maze file, if it exists, is in the correct format*/
/*
int Find_path_from_top_entrance_to_bottom_exit(char *mazefile, char *directionfile, char *visitedfile)
{
   FILE *mfptr = fopen(mazefile, "r");
   if (mfptr == NULL) {
      return -1;
   }
   
   /* read the maze from mfptr 

   Maze *maze = Read_maze_from_2Dfile(mfptr);
   fclose(mfptr);
   if (maze == NULL) {
      return -1;
   }
   
   /* find the starting location, the top-most, left-most opening 
   int startr, startc;
   startr = 0;
   for (startc = 0; startc < maze->ncol; startc++) {
      if (maze->maze_array[startr][startc] == PATH) {
         break;
      }
   }

   /* find the ending location, the bottom-most, right most opening 
   int endr, endc;
   endr = maze->nrow-1;
   for (endc = maze->ncol-1; endc >= 0; endc--) {
      if (maze->maze_array[endr][endc] == PATH) {
         break;
      }
   } 

   // no starting location, deallocate and return 
   if (startc == maze->ncol) {
      Deallocate_maze_space(maze);
      return -1;
   }
   // no ending location, deallocate and return 
   if (endc == -1) {
      Deallocate_maze_space(maze);
      return -1;
   }

   FILE *dfptr = fopen(directionfile, "w");
   if (dfptr == NULL) {
      Deallocate_maze_space(maze);
      return -1;
   }
   int steps = Pathfinder_helper(maze, startr, startc, endr, endc, 0, dfptr);
   Write_maze_to_2Dfile(visitedfile, maze);
   fclose(dfptr);
   Deallocate_maze_space(maze);
   return steps;
}
*/
/* Details of the function can be found in README                        */
/* this function expects four parameters: two filenames, and coordinates */
/* of two locations                                                      */
/* two filenames: first one is a given maze, the second one is to        */
/* be written with the directions                                        */
/* coordinates of two locations: source and destination locations        */
/*                                                                       */
/* If the maze file does not exist, you should immediately return -1 and */
/* do nothing else, i.e., the direction file should not be created       */
/* If the maze file cannot be read into a Maze structure, you should     */
/* immediately return -1 and do nothing else                             */
/* If the coordinates are not valid, do nothing and return -1            */
/* If the directions file cannot be opened for writing, return -1        */
/* The function should write the directions of a shortest path from the  */
/* source location to the destination location into the directions file  */
/* the directions are in the form of a series of characters              */
/* 'N', 'S', 'E', 'W', without spacing (and without the quotes).         */
/* There should be just a single line, without a terminating newline     */
/* character. In other words, to the vi editor, it is an incomplete file */
/* The directions should allow an agent to travel in the maze from the   */
/* source coordinates to the destination coordinates using the fewest    */
/* number of moves                                                       */
/* the number of moves taken by the agent in a shortest path should be   */
/* returned                                                              */
/* Note that the maze can be generated by amaze, or some combinations of */
/* the maze expansion functions that you have written in PE08            */
/* you may assume that the maze file, if it exists, is in the correct    */
/* format                                                                */


static int Path_helper(Maze *maze, int curr, int curc, int endr, int endc, int count, FILE * dfptr, distance *dist)
{	
	if ((dist->coordinate[curr][curc] == -5) || (dist->coordinate[curr][curc] > count)) {
		dist->coordinate[curr][curc] = count + 1 ;
		printf("(%d, %d), %d\n", curr, curc, dist->coordinate[curr][curc]);
	}
	maze->maze_array[curr][curc] = VISITED ;	
	if ((curr == endr) && (curc == endc)) { // reach the destination 
      		int i;
      		for (i = 0; i < count; i++) {   // create enough space in file
         		fputc(VISITED, dfptr);
      		}
      		//return count;
   	}

	
   	int found;
   	if (Is_path(maze, curr-1, curc)) {
	      	found = Path_helper(maze, curr-1, curc, endr, endc, count+1, dfptr, dist);
		if ((Is_path(maze, curr-1, curc)) || (Is_path(maze, curr+1, curc)) || (Is_path(maze, curr, curc-1)) || (Is_path(maze, curr, curc+1))) {
			found = -1 ;
		}
	      	if (found != -1) {
	       	 	fseek(dfptr, -1, SEEK_CUR); // go back one position
		 	fputc('N', dfptr);
		 	fseek(dfptr, -1, SEEK_CUR); // go back one position
			return found ;
      		}
   	}
   	if (Is_path(maze, curr+1, curc)) {
	      	found = Path_helper(maze, curr+1, curc, endr, endc, count+1, dfptr, dist);
		if ((Is_path(maze, curr-1, curc)) || (Is_path(maze, curr+1, curc)) || (Is_path(maze, curr, curc-1)) || (Is_path(maze, curr, curc+1))) {
			found = -1 ;
		}
	      	if (found != -1) {
		 	fseek(dfptr, -1, SEEK_CUR); // go back one position
		 	fputc('S', dfptr);
		 	fseek(dfptr, -1, SEEK_CUR); // go back one position
			return found ;      		
		}
   	}
   	if (Is_path(maze, curr, curc+1)) {
	      	found = Path_helper(maze, curr, curc+1, endr, endc, count+1, dfptr, dist);
		if ((Is_path(maze, curr-1, curc)) || (Is_path(maze, curr+1, curc)) || (Is_path(maze, curr, curc-1)) || (Is_path(maze, curr, curc+1))) {
			found = -1 ;
		}	    
	  	if (found != -1) {
		 	fseek(dfptr, -1, SEEK_CUR); // go back one position
			fputc('E', dfptr);
		 	fseek(dfptr, -1, SEEK_CUR); // go back one position
			return found ;      		
		}
   	}
   	if (Is_path(maze, curr, curc-1)) {
	      	found = Path_helper(maze, curr, curc-1, endr, endc, count+1, dfptr, dist);
		if ((Is_path(maze, curr-1, curc)) || (Is_path(maze, curr+1, curc)) || (Is_path(maze, curr, curc-1)) || (Is_path(maze, curr, curc+1))) {
			found = -1 ;
		}	
	      	if (found != -1) {
			fseek(dfptr, -1, SEEK_CUR); // go back one position
			fputc('W', dfptr);	 	
			fseek(dfptr, -1, SEEK_CUR); // go back one position
			return found ;      		
		}	
   	}
	maze->maze_array[curr][curc] = PATH ;
   	return -1;
}
		
	

int DFS_shortest_path_directions(char *mazefile, char *directionfile,
                        Coord source, Coord destination)
{
	FILE * maze_file = fopen(mazefile, "r") ;
	if (maze_file == NULL) {
		return -1 ;
	}
	Maze *maze = Read_maze_from_2Dfile(maze_file) ;
	fclose(maze_file) ;	
	if (maze == NULL) {
		Deallocate_maze_space(maze);
		return -1 ;
	}
	if ((source.row > maze->nrow) || (destination.row > maze->nrow) || (source.col > maze->ncol) || (destination.col > maze->ncol)) {
		Deallocate_maze_space(maze);		
		return -1 ;
	}
	if ((Is_path(maze, source.row, source.col) == 0) || (Is_path(maze, source.row, source.col) == 0)) {
		Deallocate_maze_space(maze);		
		return -1 ;
	}
	FILE *dirc_file = fopen(directionfile, "w") ;
	if (dirc_file == NULL) {
		Deallocate_maze_space(maze);
		return -1 ;
	}
	distance dist ;
	dist.coordinate = (int **)calloc((maze->nrow), sizeof(int *)) ;
	if (dist.coordinate == NULL) {
		fclose(dirc_file) ;
		Deallocate_maze_space(maze);
		return -1 ;
	}	
	int i = 0 ;
	int j = 0 ;
	for (i = 0; i < maze->nrow; i++) {
		dist.coordinate[i] = (int *)calloc((maze->ncol), sizeof(int)) ;
		if (dist.coordinate[i] == NULL) {
			for (j = 0; j < i; j++) {
				free(dist.coordinate[j]) ;
			}
			free(dist.coordinate) ;
			fclose(dirc_file) ;
			Deallocate_maze_space(maze);
			return -1 ;
		}
	}
	for (i = 0; i < maze->nrow; i++) {
		for (j = 0; j < maze->ncol; j++) {
			if (Is_path(maze, i, j)) {
				dist.coordinate[i][j] = -5 ;
			}
		}
	}
	////////PRINT///////////////////////////////////////////////
	for (i = 0; i < maze->nrow; i++) {			////
		for (j = 0; j < maze->ncol; j++) {		////
			printf("%.2d ", dist.coordinate[i][j]) ;////
		}						////
		printf("\n") ;					////
	}							////
	////////PRINT///////////////////////////////////////////////

	int steps = Path_helper(maze, source.row, source.col, destination.row, destination.col, 0, dirc_file, &dist); 
	fclose(dirc_file) ;

	////////PRINT///////////////////////////////////////////////
	for (i = 0; i < maze->nrow; i++) {			////
		for (j = 0; j < maze->ncol; j++) {		////
			printf("%.2d ", dist.coordinate[i][j]) ;////
		}						////
		printf("\n") ;					////
	}							////
	////////PRINT///////////////////////////////////////////////
	for (i = 0; i < maze->nrow; i++) {
		free(dist.coordinate[i]) ;
	}
	free(dist.coordinate) ;
	Deallocate_maze_space(maze);	
	 
   	return ((dist.coordinate[destination.row][destination.col]) - 1);
}

/* Details of the function can be found in README                        */
/* this function expects 5 parameters: 3 filenames, and coordinates of   */
/* the source and destination locations                                  */
/* three filenames: first one is a given maze, the second one is a       */
/* directions file (a series of characters 'N', 'S', 'E', 'W'), you are  */
/* to simulate the movement of agent according to the directions         */
/* given in the direction files. The maze showing the locations that the */
/* agent has visited will be stored in the third file.                   */
/* the source and destination coordinates are the starting and end points*/
/* of the agent's path in the maze                                       */
/*                                                                       */
/* If the maze file does not exist, you should immediately return -1 and */
/* do nothing else                                                       */
/* If the maze file cannot be read into a Maze structure, you should     */
/* immediately return -1 and do nothing else                             */
/* If the direction file does not exist, you should immediately return   */
/* -1 and do nothing else                                                */
/* if the coordinates are not valid, you should immediately return -1    */
/* and do nothing else                                                   */
/* Otherwise, it means that the agent has been airdropped to the starting*/
/* and the starting location has been visited                            */
/* You should simulate until you reach the end of file of the direction  */
/* file or encounter an invalid direction                                */
/* you should print the visited maze into the third given filename, the  */
/* visitedfile, with visited locations represented by '.'                */
/* if you encounter an invalid direction (wrong character, try to visit  */
/* a location with WALL, try to visit a location again, try to move the  */
/* agent out of bound), you should return -1                             */
/* do not change a location with WALL to a VISITED location              */
/* If the agent successfully reach the final destination with valid      */
/* directions in the direction file, and the visited maze can be output  */
/* properly, you should return the number of visited locations,          */
/* otherwise, return -1                                                  */
/* you may assume that the maze file, if it exists, is in correct format */
int Simulate_movement(char *mazefile, char *directionfile, char *visitedfile,
                      Coord source, Coord destination)
{
	int row = source.row ;
	int col = source.col ;
	int row_end = destination.row ;
	int col_end = destination.col ;
	FILE *maze_input = fopen(mazefile, "r") ;
	if (maze_input == NULL) {
		return -1 ;
	}
	FILE *dirc_input = fopen(directionfile, "r") ;
	if (dirc_input == NULL) {
		printf("hi") ;		
		fclose(maze_input) ;		
		return -1 ;
	}
	FILE *visit_file = fopen(visitedfile, "w") ;
	if (visit_file == NULL) {
		fclose(maze_input) ;		
		fclose(dirc_input) ;
		return -1 ;
	}
	Maze *maze = Read_maze_from_2Dfile(maze_input) ;
	fclose(maze_input) ;
	if (maze == NULL) {
		fclose(dirc_input) ;
		fclose(visit_file) ;
		return -1 ;
	}
	if ((source.row > maze->nrow) || (destination.row > maze->nrow) || (source.col > maze->ncol) || (destination.col > maze->ncol)) {		
		fclose(dirc_input) ;
		fclose(visit_file) ;
		Deallocate_maze_space(maze);		
		return -1 ;
	}
	if ((Is_path(maze, source.row, source.col) == 0) || (Is_path(maze, source.row, source.col) == 0)) {	
		fclose(dirc_input) ;
		fclose(visit_file) ;
		Deallocate_maze_space(maze);		
		return -1 ;
	}
	fseek(dirc_input, 0, SEEK_END) ;
	int file_size = ftell(dirc_input) ;
	fseek(dirc_input, 0, SEEK_SET) ;
	maze->maze_array[row][col] = VISITED ;
	int i = 0;
	int cnt ;
	for (i = 0; i < file_size; i++) {
		if ((cnt = fgetc(dirc_input)) != EOF) {
			if (cnt == 'N') {	
				if (Is_path(maze, row - 1, col)) {
					maze->maze_array[row - 1][col] = VISITED ;
					row = row - 1 ;
				}
				else {
					fclose(dirc_input) ;
					fclose(visit_file) ;
					Deallocate_maze_space(maze);
					return -1;
				}
			}
			else if (cnt == 'S') {
				if (Is_path(maze, row + 1, col)) {
					maze->maze_array[row + 1][col] = VISITED ;
					row = row + 1 ;
				}
				else {
					fclose(dirc_input) ;
					fclose(visit_file) ;
					Deallocate_maze_space(maze);
					return -1;
				}			
			}
			else if (cnt == 'E') {
				if (Is_path(maze, row, col + 1)) {
					maze->maze_array[row][col + 1] = VISITED ;
					col = col + 1 ;
				}
				else {
					fclose(dirc_input) ;
					fclose(visit_file) ;
					Deallocate_maze_space(maze);
					return -1;
				}
			}
			else if (cnt == 'W') {
				if (Is_path(maze, row, col - 1)) {
					maze->maze_array[row][col - 1] = VISITED ;
					col = col - 1 ;
				}
				else {
					fclose(dirc_input) ;
					fclose(visit_file) ;
					Deallocate_maze_space(maze);
					return -1;
				}
			}
			else {
				fclose(dirc_input) ;
				fclose(visit_file) ;
				Deallocate_maze_space(maze);	
				return -1;
			}
		}
	}
	if ((row == row_end) && (col == col_end)) {
		Write_maze_to_2Dfile(visitedfile, maze) ;
	}
	
	fclose(dirc_input) ;
	fclose(visit_file) ;
	Deallocate_maze_space(maze);
   	return file_size;
}


