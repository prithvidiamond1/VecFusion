void s212(int n, float *a, float *b, float *c, float *d) {
    // First loop: compute b[i] += a[i+1] * d[i] using original a[i+1] values
    for (int i = 0; i < n-1; i++) {
        b[i] += a[i+1] * d[i];
    }
    
    // Second loop: compute a[i] *= c[i] (independent)
    for (int i = 0; i < n-1; i++) {
        a[i] *= c[i];
    }
}