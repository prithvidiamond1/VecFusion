void s212(int n, float *a, float *b, float *c, float *d) {
    // Store original a[i+1] values before modifying a[i]
    for (int i = 0; i < n - 1; i++) {
        float a_next_original = a[i + 1];
        a[i] *= c[i];
        b[i] += a_next_original * d[i];
    }
}