/**
 * 2mm.c: This file is part of the PolyBench/C 3.2 test suite.
 *
 *
 * Contact: Louis-Noel Pouchet <pouchet@cse.ohio-state.edu>
 * Web address: http://polybench.sourceforge.net
 */

#include "spmv.h"
#include <stdio.h>
#include <stdlib.h>

int spmv(inout_int_t A[NxN], in_int_t alpha, in_int_t row[NxN], in_int_t col[NxN])
{
int k;
int ptr = 0;
    for (k = 1; k < N; k++) {
      for (int p = 0; p < N; p++) {
        A[row[ptr]] += alpha * A[col[ptr]];
        ptr++;
      }
    }

    return k;

}



#define AMOUNT_OF_TEST 1

int main(void){
    in_int_t alpha[AMOUNT_OF_TEST];
    in_int_t row[AMOUNT_OF_TEST][NxN];
    in_int_t col[AMOUNT_OF_TEST][NxN];
    inout_int_t A[AMOUNT_OF_TEST][NxN];
    

    for(int i = 0; i < AMOUNT_OF_TEST; ++i){
        alpha[i] = rand();
        int ptr = 0;
        for(int y = 0; y < N; ++y){
            for(int x = 0; x < N; ++x){
                A[i][ptr] = rand()%100;
                row[i][ptr] = 2; //ptr;
                col[i][ptr] = 2; //ptr;
                ptr++;
            }
        }
    }

	//for(int i = 0; i < AMOUNT_OF_TEST; ++i){
		//int i = 0;
        spmv(A[0], alpha[0], row[0], col[0]);
	//}
	

}


