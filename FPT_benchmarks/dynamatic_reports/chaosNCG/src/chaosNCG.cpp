
//------------------------------------------------------------------------
// chaosNCG
//------------------------------------------------------------------------


#include <stdlib.h>
#include "chaosNCG.h"

#define AMOUNT_OF_TEST 1

int chaosNCG(inout_int_t A[3000], in_int_t M[3000], in_int_t I, in_int_t Y, in_int_t X) 
{
	int bound = 2 * N;
  int i;

    for (i = 0; i < bound; i += 2) {
      int a = M[I + i + 2], b = M[I + i -2];
      int b0 = A[a], b1 = A[b];

      // The original code uses '%17', but this doesn't seem
      // to work in Dynamatic, so change to %16.
      // Dynamatic doesn't seem to optimize this to '&15', so
      // the benchmark is still valid.
      b0 ^= b1;
      // b0 ^= (X << ((b0 % 17)) | X >> (16 - (b0 % 17))); // This deadlocks
      b0 ^= (X << ((b0 % 16)) | X >> (16 - (b0 % 16))); // This simulates but doesn't synthesize
      // b0 ^= (X << ((b0 & 15)) | X >> (16 - (b0 & 15))); // Use &15 just for synthesis

      b1 ^= b0;
      b1 += (Y << ((b1 % 16)) | Y >> (16 - (b1 % 16)));
      // b1 += (Y << ((b1 & 15)) | Y >> (16 - (b1 & 15)));

      A[M[i + 0]] = b0;
      A[M[i + 1]] = b1;
    }

  return i;
}


int main(void){
	  in_int_t M[AMOUNT_OF_TEST][3000];
	  inout_int_t A[AMOUNT_OF_TEST][3000];
    
	srand(13);
	for(int i = 0; i < AMOUNT_OF_TEST; ++i){
    for(int j = 0; j < 3000; ++j){
      // feature[i][j] = rand()%400;
      // if (i%2==0)
        // feature[i][j] = rand()%100 < 50 ? 1 : j;
      // else
        M[i][j] = j; // 2

      A[i][j] = rand()%100;
    }
	}

	//for(int i = 0; i < AMOUNT_OF_TEST; ++i){
		int i = 0; 
		chaosNCG(A[i], M[i], 2, 1, 1);
	//}
}


