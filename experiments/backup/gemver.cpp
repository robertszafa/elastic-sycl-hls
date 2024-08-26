void gemver(in_int_t alpha, in_int_t beta, in_int_t u1[N], in_int_t v1[N],
            in_int_t u2[N], in_int_t v2[N], in_int_t y[N], in_int_t z[N],
            inout_int_t a[N][N], inout_int_t w[N], inout_int_t x[N]) {
  for (unsigned i = 0; i < N; i++)
    for (unsigned j = 0; j < N; j++)
      a[i][j] = a[i][j] + u1[i] * v1[j] + u2[i] * v2[j];

  for (unsigned i = 0; i < N; i++) {
    int tmp = x[i];
    for (unsigned j = 0; j < N; j++)
      tmp = tmp + beta * a[j][i] * y[j];
    x[i] = tmp;
  }

  for (unsigned i = 0; i < N; i++)
    x[i] = x[i] + z[i];

  for (unsigned i = 0; i < N; i++) {
    int tmp = w[i];
    for (unsigned j = 0; j < N; j++)
      tmp = tmp + alpha * a[i][j] * x[j];
    w[i] = tmp;
  }
}