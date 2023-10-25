
//------------------------------------------------------------------------
// bitonic_sort
//------------------------------------------------------------------------


#include <stdlib.h>
#include "bitonic_sort.h"

#define AMOUNT_OF_TEST 1

int bitonic_sort(inout_int_t A[N]) 
{
  int k;
    for (k = 2; k <= N; k <<= 1) { // k is doubled every iteration
      for (int j = k >> 1; j > 0; j >>= 1) { // j is halved at every iteration
        for (int i = 0; i < N; i++) {
          int l = i ^ j; 
          
          int Ai = A[i];
          int Al = A[l];
          if (l > i) {
            // Deadlock if loads inside if-condition
            // int Ai = A[i];
            // int Al = A[l];

            // Compile error if one if-condition
            // if (((i & k) == 0 && (Ai > Al)) ||
            //     ((i & k) != 0 && (Ai < Al))) {

            int toSwap = 0;
            if ((i & k) == 0) {
              if (Ai > Al) {
                toSwap = 1;
              }
            } 
            else {
              if (Ai < Al) {
                toSwap = 1;
              }             
            }

            if (toSwap == 1) {
              A[i] = Al;
              A[l] = Ai;
            }
          }
        }
      }
    }
  return k;
}


int main(void){
	  inout_int_t A[AMOUNT_OF_TEST][N];

    
	srand(13);
	for(int i = 0; i < AMOUNT_OF_TEST; ++i){
    for(int j = 0; j < N; ++j){
      A[i][j] = rand();
      // A[i][j] = j;
      // A[i][j] = N-j;
    }
	}

	//for(int i = 0; i < AMOUNT_OF_TEST; ++i){
		int i = 0; 
		bitonic_sort(A[i]);
	//}
}


