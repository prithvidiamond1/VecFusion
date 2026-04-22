void s244_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d) {
    for (int nl = 0; nl < iterations; nl++) {
        // Store original a[i+1] values to break dependency
        float* a_next = (float*)malloc((LEN_1D-1) * sizeof(float));
        for (int i = 0; i < LEN_1D-1; ++i) {
            a_next[i] = a[i+1];
        }
        
        // First compute all a[i] = b[i] + c[i] * d[i]
        for (int i = 0; i < LEN_1D-1; ++i) {
            a[i] = b[i] + c[i] * d[i];
        }
        
        // Then compute all b[i] = c[i] + b[i]
        for (int i = 0; i < LEN_1D-1; ++i) {
            b[i] = c[i] + b[i];
        }
        
        // Finally compute a[i+1] = b[i] + original_a[i+1] * d[i]
        for (int i = 0; i < LEN_1D-1; ++i) {
            a[i+1] = b[i] + a_next[i] * d[i];
        }
        
        free(a_next);
    }
}
