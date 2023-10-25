/**
 * 2mm.c: This file is part of the PolyBench/C 3.2 test suite.
 *
 *
 * Contact: Louis-Noel Pouchet <pouchet@cse.ohio-state.edu>
 * Web address: http://polybench.sourceforge.net
 */

#include "matching.h"
#include <stdio.h>
#include <stdlib.h>

int matching(inout_int_t vertices[M], in_int_t edges[M])
{
int i = 0;
    int out_scalar = 0;

    while (i < N) {
      int j = i * 2;

      int e1 = edges[j];
      int e2 = edges[j + 1];

      auto v1 = vertices[e1];
      auto v2 = vertices[e2];
      if (v1 == 0 && v2 == 0) {
        vertices[e1] = 1;
        vertices[e2] = 1;

        out_scalar = out_scalar + 1;
      }

      i = i + 1;
    }

    return out_scalar;
}



#define AMOUNT_OF_TEST 1

int main(void){
    in_int_t edges[AMOUNT_OF_TEST][M];
    inout_int_t vertices[AMOUNT_OF_TEST][M];
    

    for(int i = 0; i < AMOUNT_OF_TEST; ++i){
        for(int y = 0; y < M; ++y){
            edges[i][y] = y;
            vertices[i][y] = 0;
        }
    }

	//for(int i = 0; i < AMOUNT_OF_TEST; ++i){
		//int i = 0;
        matching(vertices[0], edges[0]);
	//}
	

}


