#include <cmath>

/*
** Constant times a vector plus a vector.
** Jack Dongarra, linpack, 3/11/78.
** ROLLED version
*/
static void daxpy_r(int n, float da, float *dx, int incx, float *dy, int incy)

{
  int i, ix, iy;

  if (n <= 0)
    return;
  if (da == 0.0f)
    return;

  if (incx != 1 || incy != 1) {

    /* code for unequal increments or equal increments != 1 */

    ix = 1;
    iy = 1;
    if (incx < 0)
      ix = (-n + 1) * incx + 1;
    if (incy < 0)
      iy = (-n + 1) * incy + 1;
    for (i = 0; i < n; i++) {
      dy[iy] = dy[iy] + da * dx[ix];
      ix = ix + incx;
      iy = iy + incy;
    }
    return;
  }

  /* code for both increments equal to 1 */

  for (i = 0; i < n; i++)
    dy[i] = dy[i] + da * dx[i];
}

/*
** Scales a vector by a constant.
** Jack Dongarra, linpack, 3/11/78.
** ROLLED version
*/
static void dscal_r(int n, float da, float *dx, int incx)

{
  int i, nincx;

  if (n <= 0)
    return;
  if (incx != 1) {

    /* code for increment not equal to 1 */

    nincx = n * incx;
    for (i = 0; i < nincx; i = i + incx)
      dx[i] = da * dx[i];
    return;
  }

  /* code for increment equal to 1 */

  for (i = 0; i < n; i++)
    dx[i] = da * dx[i];
}

/*
** constant times a vector plus a vector.
** Jack Dongarra, linpack, 3/11/78.
** UNROLLED version
*/
static void daxpy_ur(int n, float da, float *dx, int incx, float *dy, int incy)

{
  int i, ix, iy, m;

  if (n <= 0)
    return;
  if (da == 0.0f)
    return;

  if (incx != 1 || incy != 1) {

    /* code for unequal increments or equal increments != 1 */

    ix = 1;
    iy = 1;
    if (incx < 0)
      ix = (-n + 1) * incx + 1;
    if (incy < 0)
      iy = (-n + 1) * incy + 1;
    for (i = 0; i < n; i++) {
      dy[iy] = dy[iy] + da * dx[ix];
      ix = ix + incx;
      iy = iy + incy;
    }
    return;
  }

  /* code for both increments equal to 1 */

  m = n % 4;
  if (m != 0) {
    for (i = 0; i < m; i++)
      dy[i] = dy[i] + da * dx[i];
    if (n < 4)
      return;
  }
  for (i = m; i < n; i = i + 4) {
    dy[i] = dy[i] + da * dx[i];
    dy[i + 1] = dy[i + 1] + da * dx[i + 1];
    dy[i + 2] = dy[i + 2] + da * dx[i + 2];
    dy[i + 3] = dy[i + 3] + da * dx[i + 3];
  }
}

/*
** Scales a vector by a constant.
** Jack Dongarra, linpack, 3/11/78.
** UNROLLED version
*/
static void dscal_ur(int n, float da, float *dx, int incx)

{
  int i, m, nincx;

  if (n <= 0)
    return;
  if (incx != 1) {

    /* code for increment not equal to 1 */

    nincx = n * incx;
    for (i = 0; i < nincx; i = i + incx)
      dx[i] = da * dx[i];
    return;
  }

  /* code for increment equal to 1 */

  m = n % 5;
  if (m != 0) {
    for (i = 0; i < m; i++)
      dx[i] = da * dx[i];
    if (n < 5)
      return;
  }
  for (i = m; i < n; i = i + 5) {
    dx[i] = da * dx[i];
    dx[i + 1] = da * dx[i + 1];
    dx[i + 2] = da * dx[i + 2];
    dx[i + 3] = da * dx[i + 3];
    dx[i + 4] = da * dx[i + 4];
  }
}

/*
** Finds the index of element having max. absolute value.
** Jack Dongarra, linpack, 3/11/78.
*/
int idamax(int n, float *dx, int incx)

{
  float dmax;
  int i, ix, itemp;

  if (n < 1)
    return (-1);
  if (n == 1)
    return (0);
  if (incx != 1) {

    /* code for increment not equal to 1 */

    ix = 1;
    dmax = fabs((double)dx[0]);
    ix = ix + incx;
    for (i = 1; i < n; i++) {
      if (fabs((double)dx[ix]) > dmax) {
        itemp = i;
        dmax = fabs((double)dx[ix]);
      }
      ix = ix + incx;
    }
  } else {

    /* code for increment equal to 1 */

    itemp = 0;
    dmax = fabs((double)dx[0]);
    for (i = 1; i < n; i++)
      if (fabs((double)dx[i]) > dmax) {
        itemp = i;
        dmax = fabs((double)dx[i]);
      }
  }
  return (itemp);
}

void dgefa(float *a, int lda, int n) {
  float t;
  int j, k, kp1, l, nm1;

  /* gaussian elimination with partial pivoting */
  nm1 = n - 1;
  for (k = 0; k < nm1; k++) {
    kp1 = k + 1;

    /* find l = pivot index */

    l = idamax(n - k, &a[lda * k + k], 1) + k;

    /* 0.0f pivot implies this column already
       triangularized */

    if (a[lda * k + l] != 0.0f) {
      /* interchange if necessary */

      if (l != k) {
        t = a[lda * k + l];
        a[lda * k + l] = a[lda * k + k];
        a[lda * k + k] = t;
      }

      /* compute multipliers */

      t = -1.0f / a[lda * k + k];
      dscal_ur(n - (k + 1), t, &a[lda * k + k + 1], 1);

      /* row elimination with column indexing */

      for (j = kp1; j < n; j++) {
        t = a[lda * j + l];
        if (l != k) {
          a[lda * j + l] = a[lda * j + k];
          a[lda * j + k] = t;
        }

        daxpy_ur(n - (k + 1), t, &a[lda * k + k + 1], 1, &a[lda * j + k + 1],
                 1);
      }
    }
  }
}
