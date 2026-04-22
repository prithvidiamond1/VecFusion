void s212_opt(int n, float *a, float *b, float *c, float *d) {
    // Store original a[i+1] values before they are modified
    for (int i = 0; i < n - 1; i++) {
        float temp_a_next = a[i + 1];
        a[i] *= c[i];
        b[i] += temp_a_next * d[i];
    }
}
