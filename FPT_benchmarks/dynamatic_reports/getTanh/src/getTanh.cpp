#include "getTanh.h"
//------------------------------------------------------------------------
// getTanh
//------------------------------------------------------------------------

//SEPARATOR_FOR_MAIN

#include <stdlib.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>


#define AMOUNT_OF_TEST 1

int getTanh (inout_int_t A[1000], in_int_t addr[1000]) { //, in_int_t atanh[12], in_int_t cosh[5], in_int_t sinh[5]) {
	int result;	
    for (int i = 0; i < 1000; i++) {
      // Input angle
      // Result of tanh, sinh and cosh
    	result = 4096; // Saturation effect
      int beta = A[addr[i]];

      // Implement approximate range of the hyperbolic CORDIC block
      // if (beta < 20480) {  // This if condition fails to compile
        int x = 0x1351;
        int y = 1;
        int x_new = 0;
        int index_trigo = 0;
        int result_cosh = 0, result_sinh = 0;
        int outputcosh = 0, outputsinh = 0;

        // Index trigo should be used as index into ROMs,
        // but since passing the ROMs fails to compile use the index directly   
        if (beta >= 8192) {
          index_trigo = 4;
        } else if (beta >= 12288) {
          index_trigo = 3;
        } else if (beta >= 8192) {
          index_trigo = 2;
        } else if (beta >= 4096) {
          index_trigo = 5;
        } else {
          index_trigo = 6;
        }
        beta = beta - index_trigo * 4096;

        // Unrolling is not supported.
        // Manual unrolling fails to compile beyond 3x unroll.

        // Call to the hyperbolic CORDIC block
        #pragma unroll
        for (int k = 1; k <= 12; k++) { 
          // force the 3k+1 th iteration to be repeated
          // index_trigo += (k%3);
          if (((k % 3) == 1) && (k != 1)) { // This if condition fails to compile
          // if (k == 4 || k == 8 || k == 12) { 
                x_new = x - (y >> k);
                y -= x >> k;
                beta += index_trigo;
              x = x_new;
                x_new = x - (y >> k);
                y -= x >> k;
                beta += index_trigo;
              x = x_new;

            #pragma unroll
            for (int j = 1; j <= 2; j++) {
              // beta<0 anti-clockwise rotation
              if (beta < 0) {
                x_new = x - (y >> k);
                y -= x >> k;
                beta += index_trigo;
              }
              // beta>0 clockwise rotation
              else {
                x_new = x + (y >> k);
                y += (x >> k);
                beta -= index_trigo;
              }
              x = x_new;
            }
          } 
          else {
            if (beta < 0) {
              x_new = x - (y >> k);
              y -= x >> k;
              beta += index_trigo;
            }
            // beta>0 clockwise rotation
            else {
              x_new = x + (y >> k);
              y += (x >> k);
              beta -= index_trigo;
            }
            x = x_new;
          }
        }
        outputcosh = x;
        outputsinh = y;

        // Trigonometric rules application
        result_cosh =
            // (sinh[index_trigo] * outputcosh + cosh[index_trigo] * outputsinh);
            (index_trigo * outputcosh + index_trigo * outputsinh);
        result_sinh =
            // (cosh[index_trigo] * outputcosh + sinh[index_trigo] * outputsinh) >>
            (index_trigo * outputcosh + index_trigo * outputsinh) >>
            12;
        // result = result_cosh / result_sinh; // Fails to compile
        result = (result_cosh | 1) / ((result_sinh) | 1); // change division if you want to synthesize
        // result = result_cosh * result_sinh * result_cosh * result_sinh * result_cosh;
      // }

      // Central symmetry correction
      A[addr[i]] = result;
    }

	return result;
}

#define AMOUNT_OF_TEST 1

int main(void){
	inout_int_t a[AMOUNT_OF_TEST][1000];
	in_int_t b[AMOUNT_OF_TEST][1000];

	int atanh[1][12] = {{0x08C9, 0x0416, 0x0202, 0x0100, 0x0080, 0x0064,
                     0x0032, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001}};
    int cosh[1][5] = {{0x1000, 0x18B0, 0x3C31, 0xA115, 0x1B4EE}};
    int sinh[1][5] = {{0x0, 0x12CD, 0x3A07, 0xA049, 0x1B4A3}};

	for(int i = 0; i < AMOUNT_OF_TEST; ++i){
		for(int j = 0; j < N; ++j){
    		a[i][j] = j;//(float) j;
			b[i][j] = j; // j

			// if (j%100 == 0)
			   	a[i][j] = 0;
		}
	}

	//for(int i = 0; i < AMOUNT_OF_TEST; ++i){
	int i = 0;
	//gsum(a[i]);
  // Passing the atanh,sinh,cosh arrays fails to compile  
	getTanh (a[i], b[i]); //, atanh[i], cosh[i], sinh[i]);

}



