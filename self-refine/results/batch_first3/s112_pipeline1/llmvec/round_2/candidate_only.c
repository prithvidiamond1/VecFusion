void vectorized_s112(float *a, float *b, int *c, int LEN_1D) {
    for (int i = LEN_1D - 2; i >= 0; i--) {
        a[i + 1] = a[i] + b[i];
    }
}
