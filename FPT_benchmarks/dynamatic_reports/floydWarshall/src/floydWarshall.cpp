
//------------------------------------------------------------------------
// floydWarshall
//------------------------------------------------------------------------


#include <stdlib.h>
#include "floydWarshall.h"

#define AMOUNT_OF_TEST 1

int floydWarshall(inout_int_t dist[NxN]) 
{
  int k;
  for (k=0; k < N; ++k) {
    for (int i=0; i < N; ++i) {
      for (int j=0; j < N; ++j) {
        if (dist[i*N + j] > dist[i*N + k] + dist[k*N + j])
          dist[i*N + j] = dist[i*N + k] + dist[k*N + j];
      }
    }
  }
  return k;
}


int main(void){
	  in_int_t vertices[AMOUNT_OF_TEST][N];
	  inout_int_t dist[AMOUNT_OF_TEST][NxN];
    
	srand(13);
	for(int i = 0; i < AMOUNT_OF_TEST; ++i){
    for(int j = 0; j < N; ++j){
      for(int k = 0; k < N; ++k){
        dist[i][j*N + k] = j == k ? 0 : rand() % N; // 0
      }

      vertices[i][j] = rand()%100;
    }
	}

	//for(int i = 0; i < AMOUNT_OF_TEST; ++i){
		int i = 0; 
		floydWarshall(dist[i]);
	//}
}


