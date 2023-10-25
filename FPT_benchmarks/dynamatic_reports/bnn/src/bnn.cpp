
//------------------------------------------------------------------------
// bnn
//------------------------------------------------------------------------


#include <stdlib.h>
#include "bnn.h"

#define AMOUNT_OF_TEST 1

int bnn(in_int_t w[NxN], in_int_t in[NxN], in_int_t mean[NxN], in_int_t addr_in[NxN], in_int_t addr_out[NxN], inout_int_t data[NxN]) 
{
    int alpha = 2;

    int i, k;
    for (i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        int x = i * N + j;
        int lut = in[x] ^ w[x];
        data[addr_in[x]] += lut * alpha;
      }

      // THIS DEADLOCKS
      // if (i == (N-1)) {
        // for (k = 0; k < N; k++) {
        //   int y = i * N + k;
        //   int m = mean[y];
        //   int z = data[addr_out[y]];
        //   // if (i == (N-1)) {
        //     if (z > 0)
        //       z = z - m;
        //     else
        //       z = z + m;
        //   // }
        //   data[addr_out[y]] = z;
        // }
      // }
    }

    // THIS DOES NOT
    for (k = 0; k < N; k++) {
      int y = (i-1) * N + k;
      int m = mean[y];
      int temp = data[addr_out[y]];
      int z;
      if (temp > 0)
        z = temp - m;
      else
        z = temp + m;
      data[addr_out[y]] = z;
    }

  return i;
}


int main(void){
	  in_int_t addr_in[AMOUNT_OF_TEST][NxN];
	  in_int_t addr_out[AMOUNT_OF_TEST][NxN];
	  inout_int_t data[AMOUNT_OF_TEST][NxN];

    in_int_t w[AMOUNT_OF_TEST][NxN];
    in_int_t in[AMOUNT_OF_TEST][NxN];
    in_int_t mean[AMOUNT_OF_TEST][NxN];
    
	srand(13);
	for(int i = 0; i < AMOUNT_OF_TEST; ++i){
    for(int j = 0; j < NxN; ++j){
      // addr_in[i][j] = rand()%400;
      // if (i%2==0)
        // addr_in[i][j] = rand()%100 < 50 ? 1 : j;
      // else
        addr_in[i][j] = j; // 2 j

      addr_out[i][j] = j; // 2 // j
      data[i][j] = rand()%100;

      w[i][j] = 7;
      in[i][j] = 5;
      mean[i][j] = 3;
    }
	}

	//for(int i = 0; i < AMOUNT_OF_TEST; ++i){
		int i = 0; 
		bnn(w[i], in[i], mean[i], addr_in[i], addr_out[i], data[i]);
	//}
}


