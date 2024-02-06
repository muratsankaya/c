
#include "lab3.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

struct line{
	int valid_bit;
	int tag; 
	int block; //doesn't have to be used for the purpose of the simulator
	long int last_used; //to implement the LRU policy	
};

struct set{ 
	struct line *line_array;
};


int store(struct line *curr_line_array, int E, unsigned long tag, int verbose_mode, 
	int *hit_count, int *miss_count, int *eviction_count){
		
	for(size_t i = 0; i < E; ++i){ //first search for a tag match
		if(curr_line_array[i].tag == tag){
			++(*hit_count);
			if(verbose_mode) 
				printf(" hit");
			if(!curr_line_array[i].valid_bit) 
				curr_line_array[i].valid_bit = 1;
			return i;
		}
	}
							
	//then see if there are any unsued line
	for(size_t i = 0; i < E; ++i){
		if(!curr_line_array[i].valid_bit){
			++(*miss_count);
			curr_line_array[i].valid_bit = 1;
			curr_line_array[i].tag = tag;
			if(verbose_mode) 
				printf(" miss");
			return i;
		}
	}
			
	//if there aren't any unused lines or matching tags
	//the least used line should be evicted
	size_t lru_index = 0;
	for(size_t i = 1; i < E; ++i){
		if(curr_line_array[i].last_used < curr_line_array[lru_index].last_used)
			lru_index = i;
		
	}

	curr_line_array[lru_index].tag = tag;
	++(*eviction_count);
	++(*miss_count);
	if(verbose_mode) 
		printf(" miss eviction");
	return lru_index;
			
}

//This is currently equivalent to the store function
//however I'm keeping it for the case that there will be a need to work with data
int load(struct line *curr_line_array, int E, unsigned long tag, int verbose_mode,
        int *hit_count, int *miss_count, int *eviction_count){
	
	for(size_t i = 0; i < E; ++i){ //first search for a matching tag
		if(curr_line_array[i].tag == tag){
			if(curr_line_array[i].valid_bit){ //if it is valid line then thats a hit
				if(verbose_mode)
					printf(" hit");
				++(*hit_count);
			}
			
			//if it is not a valid line, then that is a miss but no eviction needed
			//data can be transfered to the current line 	 
			else{  
			        if(verbose_mode)
						printf(" miss");
				++(*miss_count);
				curr_line_array[i].valid_bit = 1;
			}
			return i;
		}
	}

	for(size_t i = 0; i < E; ++i){ //then search for a unused line, where the data can be transferred to
		if(!curr_line_array[i].valid_bit){ //if unused line exists use it to tranfer the data from the memory hierarchy 
			if(verbose_mode)
				printf(" miss");
			++(*miss_count);
			curr_line_array[i].valid_bit = 1;
			curr_line_array[i].tag = tag;
			return i; 
		}
	}
		
	//if there aren't any mathcing tags and unused lines
	//then the least used line must be evicted	
	size_t lru_index = 0;
	for(size_t i = 0; i < E; ++i){ 
		if(curr_line_array[i].last_used < curr_line_array[lru_index].last_used)
			lru_index = i;
	}
	if(verbose_mode)
		printf(" miss eviction");
	++(*miss_count);
	++(*eviction_count);
	curr_line_array[lru_index].tag = tag;
	return lru_index;
	
}


int main(int argc, char **argv)
{	

	int s, S, E, b, m = 64, verbose_mode = 0;

	char *iFile_str; 

	for(size_t i = 0; i < argc; ++i){
 		
		if(!strcmp(argv[i],"-v"))
			verbose_mode = 1;
		else if(!strcmp(argv[i], "-s"))
			s = atoi(argv[i+1]);
		else if(!strcmp(argv[i], "-E"))
			E = atoi(argv[i+1]);
		else if(!strcmp(argv[i], "-b"))
			b = atoi(argv[i+1]);
		else if(!strcmp(argv[i], "-t"))
			iFile_str = strtok(argv[i+1], " ");
		else if(!strcmp(argv[i],"-itest")) //strmcp returns 0 if strings are equal
			printf("s:%d E:%d b:%d vebose_mode:%d input_file: %s\n", s, E, b, verbose_mode, iFile_str);	
			
	}
	
	S = pow(2, s); //S many sets
		
	struct set *cache = (struct set*)malloc(S*sizeof(struct set));
	
	for(size_t i = 0; i < S; ++i){ //cache is an array of sets
		cache[i].line_array = (struct line*)malloc(E*sizeof(struct line)); //create E many lines per set
		for(size_t j = 0; j < E; ++j){
			cache[i].line_array[j].valid_bit = 0;
			cache[i].line_array[j].last_used = 0;
		}		
	}

	char input_line[256];
	FILE *iFile = fopen(iFile_str, "r");
	if(iFile == NULL){
		printf("Error! Could not open the file\n");
		exit(1);
	}
		
	int hit_count = 0, miss_count = 0, eviction_count = 0;		

	while(fgets(input_line, sizeof(input_line), iFile)){

		if(input_line[0] == 'I')
			continue; 		

		char *iptr = &input_line[0]; //pointer to the input_line
		while(isspace(*iptr)) ++iptr; //remove the leading whitespace 
		strtok(iptr, "\n");
						
		if(verbose_mode) 
			printf("%s ", iptr); 
      	
		char *instruction = strtok(iptr, " ");
		
		iptr += 2; //move to the beginning of input memory
		
		iptr = strtok(iptr,","); //cut the address			
			
		long unsigned mem;
		mem = strtol(iptr, NULL, 16); //contemplate the address as base 16	
		//printf("Memory Address: %x \n", mem); //to test the address

		unsigned long tag = mem, set_index = mem, block_offset = mem;
		
		size_t tag_bits_size = m - (s+b);		

		tag = tag >> (s + b);
		
		set_index = set_index << tag_bits_size;

		set_index = set_index >> (tag_bits_size + b);
	
		block_offset = block_offset << (tag_bits_size + s);
		
		block_offset = block_offset >> (tag_bits_size + s);		
		
		//printf("tag: %d, set index: %d, block offset: %d \n", tag, set_index, block_offset);		
			
		struct line *curr_line_array = cache[set_index].line_array;
		int storage_index = -1; //For LRU 
			
		if(!strcmp(instruction, "S"))
			storage_index = store(curr_line_array, E, tag, verbose_mode, &hit_count, &miss_count, &eviction_count); 
		else if(!strcmp(instruction, "L"))
			storage_index = load(curr_line_array, E, tag, verbose_mode, &hit_count, &miss_count, &eviction_count);
		else { 
			storage_index = load(curr_line_array, E, tag, verbose_mode, &hit_count, &miss_count, &eviction_count);
			store(curr_line_array, E, tag, verbose_mode, &hit_count, &miss_count, &eviction_count);
		}

		if(verbose_mode) 
			printf("\n");
	
		if(storage_index >= 0){ 
			
			for(size_t i = 0; i < E; ++i)
				if(i != storage_index)
					curr_line_array[i].last_used -= 1;
				
			curr_line_array[storage_index].last_used = 0;

		}	

		
	}
	
	printSummary(hit_count, miss_count, eviction_count);

	fclose(iFile);	

	for(size_t i = 0; i < S; ++i){ //cache is an array of sets
            free(cache[i].line_array); //free all the lines
    	}	

	free(cache); //free the cache 
   
}


