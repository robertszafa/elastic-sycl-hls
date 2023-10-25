#include "getTanh_double.h"
//------------------------------------------------------------------------
// getTanh_double
//------------------------------------------------------------------------

//SEPARATOR_FOR_MAIN

#include <stdlib.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>


#define AMOUNT_OF_TEST 1

int getTanh_double (inout_int_t A[1000], in_int_t addr[1000]) {
	// getTanh_double
	inout_int_t result, beta;
	int i;
	for(i = 0; i < 1000; i++) {
		int address = addr[i];
		beta = A[address];

		result = ((beta * beta + 19) * beta * beta + 3) * beta; //((beta*beta+(inout_int_t)19.52381)*beta*beta + (inout_int_t)3.704762)*beta;

		A[address] = result;

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
			b[i][j] = 2;

			if (j%100 == 0)
			   	a[i][j] = 0;
		}
	}

	//for(int i = 0; i < AMOUNT_OF_TEST; ++i){
	int i = 0;
	//gsum(a[i]);
	getTanh_double (a[i], b[i]);

}



