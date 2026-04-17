void s212_opt(int n, float *a, float *b, float *c, float *d) {
    for (int i = 0; i < n - 1; i++) {
        b[i] += a[i + 1] * d[i];
    }
    for (int i = 0; i < n - 1; i++) {
        a[i] *= c[i];
    }
}
