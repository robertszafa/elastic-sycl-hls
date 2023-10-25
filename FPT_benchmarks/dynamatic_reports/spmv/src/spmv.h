typedef int in_int_t;
typedef int out_int_t;
typedef int inout_int_t;

#define N 20
#define NxN 400


int spmv(inout_int_t A[NxN], in_int_t alpha, in_int_t row[NxN], in_int_t col[NxN]);
