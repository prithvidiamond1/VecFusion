void s241(int iterations, int LEN_1D, float* a, float* b, float* c, float* d) {
    float* a_old = (float*)malloc(LEN_1D * sizeof(float));
    
    for (int nl = 0; nl < 2 * iterations; nl++) {
        // Store old a values before modifying them
        for (int i = 0; i < LEN_1D; i++) {
            a_old[i] = a[i];
        }
        
        // Compute new a[i] using old b[i]
        for (int i = 0; i < LEN_1D - 1; i++) {
            a[i] = b[i] * c[i] * d[i];
        }
        
        // Compute new b[i] using new a[i] and old a[i+1]
        for (int i = 0; i < LEN_1D - 1; i++) {
            b[i] = a[i] * a_old[i + 1] * d[i];
        }
    }
    
    free(a_old);
}
