extern "C" void histogram_kernel(int *idx, int *hist) {
  for (int i = 0; i < 100; ++i) {
    int idx_scalar = idx[i];
    int x = hist[idx_scalar];
    hist[idx_scalar] = x + 3;
  }
}
