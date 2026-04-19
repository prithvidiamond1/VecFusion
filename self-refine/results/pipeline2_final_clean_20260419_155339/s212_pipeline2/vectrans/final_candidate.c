void s212_opt(int n, float *a, float *b, float *c, float *d) {
    // Store original a[i+1] values before they are modified
    float *a_next = (float *)malloc((n-1) * sizeof(float));
    if (a_next == NULL) return;
    
    // First loop: compute a[i] *= c[i] and save original a[i+1]
    for (int i = 0; i < n-1; i++) {
        a_next[i] = a[i+1];  // Save original value before modification
        a[i] *= c[i];
    }
    
    // Second loop: compute b[i] += saved_a_next * d[i]
    for (int i = 0; i < n-1; i++) {
        b[i] += a_next[i] * d[i];
    }
    
    free(a_next);
}
