// 67d7842dbbe25473c3c32b93c0da8047785f30d78e8a024de1b57352245f9689
/*******************************************************************************
Vendor: Xilinx 
Associated Filename: loop_imperfect.c
Purpose:Vivado HLS Coding Style example 
Device: All 
Revision History: May 30, 2008 - initial release
                                                
*******************************************************************************
#-  (c) Copyright 2011-2019 Xilinx, Inc. All rights reserved.
#-
#-  This file contains confidential and proprietary information
#-  of Xilinx, Inc. and is protected under U.S. and
#-  international copyright and other intellectual property
#-  laws.
#-
#-  DISCLAIMER
#-  This disclaimer is not a license and does not grant any
#-  rights to the materials distributed herewith. Except as
#-  otherwise provided in a valid license issued to you by
#-  Xilinx, and to the maximum extent permitted by applicable
#-  law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
#-  WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
#-  AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
#-  BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
#-  INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
#-  (2) Xilinx shall not be liable (whether in contract or tort,
#-  including negligence, or under any other theory of
#-  liability) for any loss or damage of any kind or nature
#-  related to, arising under or in connection with these
#-  materials, including for any direct, or any indirect,
#-  special, incidental, or consequential loss or damage
#-  (including loss of data, profits, goodwill, or any type of
#-  loss or damage suffered as a result of any action brought
#-  by a third party) even if such damage or loss was
#-  reasonably foreseeable or Xilinx had been advised of the
#-  possibility of the same.
#-
#-  CRITICAL APPLICATIONS
#-  Xilinx products are not designed or intended to be fail-
#-  safe, or for use in any application requiring fail-safe
#-  performance, such as life-support or safety devices or
#-  systems, Class III medical devices, nuclear facilities,
#-  applications related to the deployment of airbags, or any
#-  other applications that could lead to death, personal
#-  injury, or severe property or environmental damage
#-  (individually and collectively, "Critical
#-  Applications"). Customer assumes the sole risk and
#-  liability of any use of Xilinx products in Critical
#-  Applications, subject only to applicable laws and
#-  regulations governing limitations on product liability.
#-
#-  THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
#-  PART OF THIS FILE AT ALL TIMES. 
#- ************************************************************************


This file contains confidential and proprietary information of Xilinx, Inc. and 
is protected under U.S. and international copyright and other intellectual 
property laws.

DISCLAIMER
This disclaimer is not a license and does not grant any rights to the materials 
distributed herewith. Except as otherwise provided in a valid license issued to 
you by Xilinx, and to the maximum extent permitted by applicable law: 
(1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL FAULTS, AND XILINX 
HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, 
INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT, OR 
FITNESS FOR ANY PARTICULAR PURPOSE; and (2) Xilinx shall not be liable (whether 
in contract or tort, including negligence, or under any other theory of 
liability) for any loss or damage of any kind or nature related to, arising under 
or in connection with these materials, including for any direct, or any indirect, 
special, incidental, or consequential loss or damage (including loss of data, 
profits, goodwill, or any type of loss or damage suffered as a result of any 
action brought by a third party) even if such damage or loss was reasonably 
foreseeable or Xilinx had been advised of the possibility of the same.

CRITICAL APPLICATIONS
Xilinx products are not designed or intended to be fail-safe, or for use in any 
application requiring fail-safe performance, such as life-support or safety 
devices or systems, Class III medical devices, nuclear facilities, applications 
related to the deployment of airbags, or any other applications that could lead 
to death, personal injury, or severe property or environmental damage 
(individually and collectively, "Critical Applications"). Customer assumes the 
sole risk and liability of any use of Xilinx products in Critical Applications, 
subject only to applicable laws and regulations governing limitations on product 
liability. 

THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE AT 
ALL TIMES.

*******************************************************************************/
#include "loop_imperfect.h"

void loop_imperfect(dout_t A[N], din_t idx[N]) {
	int atanh[12] = {0x08C9, 0x0416, 0x0202, 0x0100, 0x0080, 0x0064,
	                     0x0032, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001};
	    int cosh[5] = {0x1000, 0x18B0, 0x3C31, 0xA115, 0x1B4EE};
	    int sinh[5] = {0x0, 0x12CD, 0x3A07, 0xA049, 0x1B4A3};

	    for (int i = 0; i < N; i++) {
	      // Input angle
	      // Result of tanh, sinh and cosh
	      int result = 4096; // Saturation effect
	      int beta = A[idx[i]];

	      // Implement approximate range of the hyperbolic CORDIC block
	      if (beta < 20480) {
	        int x = 0x1351;
	        int y = 0;
	        int x_new;
	        int index_trigo;
	        int result_cosh, result_sinh;
	        int outputcosh, outputsinh;

	        if (beta >= 8192) {
	          index_trigo = 4;
	        } else if (beta >= 12288) {
	          index_trigo = 3;
	        } else if (beta >= 8192) {
	          index_trigo = 2;
	        } else if (beta >= 4096) {
	          index_trigo = 1;
	        } else {
	          index_trigo = 0;
	        }
	        beta = beta - index_trigo * 4096;

	        // Call to the hyperbolic CORDIC block
	        for (int k = 1; k <= 12; k++) {
	        #pragma HLS unroll
	          // force the 3k+1 th iteration to be repeated
	          if (((k % 3) == 1) && (k != 1)) {
	            for (int j = 1; j <= 2; j++) {
	            #pragma HLS unroll
	              // beta<0 anti-clockwise rotation
	              if (beta < 0) {
	                x_new = x - (y >> k);
	                y -= x >> k;
	                beta += atanh[k - 1];
	              }
	              // beta>0 clockwise rotation
	              else {
	                x_new = x + (y >> k);
	                y += (x >> k);
	                beta -= atanh[k - 1];
	              }
	              x = x_new;
	            }
	          } else {
	            if (beta < 0) {
	              x_new = x - (y >> k);
	              y -= x >> k;
	              beta += atanh[k - 1];
	            }
	            // beta>0 clockwise rotation
	            else {
	              x_new = x + (y >> k);
	              y += (x >> k);
	              beta -= atanh[k - 1];
	            }
	            x = x_new;
	          }
	        }
	        outputcosh = x;
	        outputsinh = y;

	        // Trigonometric rules application
	        result_cosh =
	            (sinh[index_trigo] * outputcosh + cosh[index_trigo] * outputsinh);
	        result_sinh =
	            (cosh[index_trigo] * outputcosh + sinh[index_trigo] * outputsinh) >>
	            12;
	        result = result_cosh / result_sinh;
	      }

	      // Central symmetry correction
	      A[idx[i]] = result;
	    }
}
