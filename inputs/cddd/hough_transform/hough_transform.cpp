#ifndef M_PI
  #define M_PI 3.1415927
#endif

#define GR(X, Y) (d[(*s) * (Y) + bpp * (X) + ((2) % bpp)])
#define GG(X, Y) (d[(*s) * (Y) + bpp * (X) + ((1) % bpp)])
#define GB(X, Y) (d[(*s) * (Y) + bpp * (X) + ((0) % bpp)])
#define SR(X, Y) (ht[4 * tw * ((Y) % th) + 4 * ((X) % tw) + 2])
#define SG(X, Y) (ht[4 * tw * ((Y) % th) + 4 * ((X) % tw) + 1])
#define SB(X, Y) (ht[4 * tw * ((Y) % th) + 4 * ((X) % tw) + 0])
#define RAD(A) (M_PI * ((double)(A)) / 180.0)
uint8_t *houghtransform(uint8_t *d, int *w, int *h, int *s, int bpp) {
  int rho, theta, y, x, W = *w, H = *h;
  int th = sqrt(W * W + H * H) / 2.0;
  int tw = 360;
  uint8_t *ht = malloc(th * tw * 4);
  memset(ht, 0, 4 * th * tw); // black bg

  for (rho = 0; rho < th; rho++) {
    for (theta = 0; theta < tw /*720*/; theta++) {
      double C = cos(RAD(theta));
      double S = sin(RAD(theta));
      uint32_t totalred = 0;
      uint32_t totalgreen = 0;
      uint32_t totalblue = 0;
      uint32_t totalpix = 0;
      if (theta < 45 || (theta > 135 && theta < 225) || theta > 315) {
        for (y = 0; y < H; y++) {
          double dx = W / 2.0 + (rho - (H / 2.0 - y) * S) / C;
          if (dx < 0 || dx >= W)
            continue;
          x = floor(dx + .5);
          if (x == W)
            continue;
          totalpix++;
          totalred += GR(x, y);
          totalgreen += GG(x, y);
          totalblue += GB(x, y);
        }
      } else {
        for (x = 0; x < W; x++) {
          double dy = H / 2.0 - (rho - (x - W / 2.0) * C) / S;
          if (dy < 0 || dy >= H)
            continue;
          y = floor(dy + .5);
          if (y == H)
            continue;
          totalpix++;
          totalred += GR(x, y);
          totalgreen += GG(x, y);
          totalblue += GB(x, y);
        }
      }
      if (totalpix > 0) {
        double dp = totalpix;
        SR(theta, rho) = (int)(totalred / dp) & 0xff;
        SG(theta, rho) = (int)(totalgreen / dp) & 0xff;
        SB(theta, rho) = (int)(totalblue / dp) & 0xff;
      }
    }
  }

  *h = th; // sqrt(W*W+H*H)/2
  *w = tw; // 360
  *s = 4 * tw;
  return ht;
}
