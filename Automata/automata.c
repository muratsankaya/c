#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printGrid(short grid[5][5], short n);

short eightDirectSearch(short grid[5][5], short r, short c, short n);

void copyGrid(short s[5][5], short d[5][5], size_t n); 

int main(int argc, char **argv){
	
	if(argc != 3 || strlen(argv[2]) > 50){ 
		printf("Invalid input\n");
		return 0;
	}

	FILE *iFile;
	iFile = fopen(argv[2], "r");	 	

	if(iFile == NULL){ 
		printf("Err: couldn't read the input file! \n");
		return 0;
	}
	
	short currGeneration[5][5];

	size_t r = 0;

        while(fscanf(iFile, "%hd %hd %hd %hd %hd", &currGeneration[r][0], &currGeneration[r][1],
                                                   &currGeneration[r][2], &currGeneration[r][3],
                                                   &currGeneration[r][4]) == 5){
		              
        	++r;
	
	}

	fclose(iFile);
		
	short nextGeneration[5][5];
	
	copyGrid(currGeneration, nextGeneration, 5);
				
	for(size_t count = 0; count < atoi(argv[1]); ++count){
	//for(size_t i = 0; i < 1; ++i){
		
		for(r = 0; r < 5; ++r){
			for(size_t c = 0; c < 5; ++c){
				//printf("%hd ", eightDirectSearch(currGeneration, r, c, 5));
				short neighboorCount = eightDirectSearch(currGeneration, r, c, 5);
				
				//if the cell is alive and has less than 2 or more than 3 neighboors cell dies
				if(currGeneration[r][c] == 1 && (neighboorCount < 2 || neighboorCount > 3)){ 
					
					nextGeneration[r][c] = 0; 
					
				}
				
				//dead cell with 3 neighboors becomes alive
				else if(currGeneration[r][c] == 0 && neighboorCount == 3){
					
					nextGeneration[r][c] = 1;				

				}

			}
			//printf("\n");
		}
	
		copyGrid(nextGeneration, currGeneration, 5); 
		
	}
			
	//printGrid(currGeneration, 5);			
	char oFileStr[60];
	strcpy(oFileStr, argv[2]);
	FILE *oFile = fopen(strcat(oFileStr,".out"), "w");

	for(size_t r = 0; r < 5; ++r){
		fprintf(oFile, "%hd %hd %hd %hd %hd\n", currGeneration[r][0], currGeneration[r][1],
                                                        currGeneration[r][2], currGeneration[r][3],
                                                        currGeneration[r][4] );
	}

	fclose(oFile);

}


void printGrid(short grid[5][5], short n){ //this is only for testing purposes

	for(size_t r = 0; r < n; ++r){
		for(size_t c = 0; c < n; ++c){
			printf("%hd ", grid[r][c]);
		}
		printf("\n");
	}

}

short eightDirectSearch(short grid[5][5], short r, short c, short n){ //Does a eight-directional-search and returns count of alive neighboors
	short count = 0; //count of the live neighboors
	if(r - 1 >= 0 && c - 1 >= 0 && grid[r-1][c-1] == 1) ++count; //north-west
	if(r - 1 >= 0 && grid[r-1][c] == 1) ++count; //north
	if(r - 1 >= 0 && c + 1 < n && grid[r-1][c+1] == 1) ++count; //north-east
	if(c - 1 >= 0 && grid[r][c-1] == 1) ++count; //west
	if(c + 1 < n && grid[r][c+1] == 1) ++count; //east
	if(r + 1 < n && c - 1 >= 0 && grid[r+1][c-1] == 1) ++count; //south-west
	if(r + 1 < n && grid[r+1][c] == 1) ++count; //south
	if(r + 1 < n && c + 1 < n && grid[r+1][c+1] == 1) ++count; //south-east
  	return count;
}

void copyGrid(short s[5][5], short d[5][5], size_t n){ //copy the entries from s to d 

	for(size_t r = 0; r < n; ++r){
		for(size_t c = 0; c < n; ++c){
			d[r][c] = s[r][c];
		}
	}

}


