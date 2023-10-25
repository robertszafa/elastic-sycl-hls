#include "vecTrans.h"
//------------------------------------------------------------------------
// vecTrans
//------------------------------------------------------------------------

//SEPARATOR_FOR_MAIN

#include <stdlib.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>


#define AMOUNT_OF_TEST 1

int vecTrans (inout_int_t A[1000], in_int_t b[1000]) {
	// vecTrans
	int i;
	for (i = 0; i < N; i++) {
      int d = A[i];
      A[b[i]] =
          (((((((d + 112) * d + 23) * d + 36) * d + 82) * d + 127) * d + 2) *
               d +
           20) *
              d +
          100;
    }

	return i;
}

#define AMOUNT_OF_TEST 1

int main(void){
	inout_int_t a[AMOUNT_OF_TEST][1000];
	in_int_t b[AMOUNT_OF_TEST][1000];
    
	for(int i = 0; i < AMOUNT_OF_TEST; ++i){
		for(int j = 0; j < 1000; ++j){
    		a[i][j] = (inout_int_t) j;
			b[i][j] = (j+1) % 1000; // j

			if (j%100 == 0)
			   	a[i][j] = 0;
		}
	}

	//for(int i = 0; i < AMOUNT_OF_TEST; ++i){
	int i = 0;
	//gsum(a[i]);
	vecTrans (a[i], b[i]);

}



