

/* Murat Sankaya - ms12080 */

/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "lab3.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
	int i, ib, j, jb, tmp, tmp2, tmp3, block_size;
	//determined the block_size by trial and error for each case 
		
	if(M == 32 && N == 32){
		block_size = 8; 
		for(ib = 0; ib < N; ib += block_size){
			for(jb = 0; jb < M; jb += block_size){
				for (i = ib; i < ib + block_size; i++){
					for (j = jb; j < jb + block_size; j++){				
						if(i != j) //to decrease the misses on the diagonal entries
							B[j][i] = A[i][j];
						else 
							tmp = A[i][j];
					
					} 
					if(ib == jb)  	
						B[i][i] = tmp;  	
				}
			}
		}
	}else if(N == 64 && M == 64){
		block_size = 4;
		for(ib = 0; ib < N; ib += block_size){
                        for(jb = 0; jb < M; jb += block_size){
                                for (i = ib; i < ib + block_size; i++){
                                        for (j = jb; j < jb + block_size; j++){
         					if(i != j)
                                                	B[j][i] = A[i][j];
						else if(i - j == N - 1) //to decrease the misses on the anti-diagonal
							tmp2 = A[i][j];
						else
							tmp = A[i][j];
                                        }
					if(ib == jb)
						B[i][i] = tmp;
					if(tmp2 != tmp3){
						B[N-1-i][i] = tmp2;
						tmp3 = tmp2;	
					}	 
                              	}
                        }
                }
	}else{
		block_size = 16;
                for(ib = 0; ib < N; ib += block_size){
                        for(jb = 0; jb < M; jb += block_size){
                                for (i = ib; i < ib + block_size && i < N; i++){
                                        for (j = jb; j < jb + block_size && j < M; j++){
                                                if(i != j)
                                                        B[j][i] = A[i][j];
                                                else
                                                        tmp = A[i][j];

                                        }
		
                                        if(ib == jb) //to decrease the misses on the diagonal entries
                                                B[i][i] = tmp;
                                }
                        }
                }
	}		
}
/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

