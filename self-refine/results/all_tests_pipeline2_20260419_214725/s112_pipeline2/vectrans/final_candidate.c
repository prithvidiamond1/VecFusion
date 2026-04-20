void s112_opt(float *a, float *b, int iterations, int LEN_1D) {
    for (int nl = 0; nl < 3 * iterations; nl++) {
        // Store original values to break dependency
        float temp[LEN_1D];
        for (int i = 0; i < LEN_1D - 1; i++) {
            temp[i] = a[i];
        }
        
        // Vectorizable loop - no loop-carried dependency
        for (int i = 0; i < LEN_1D - 1; i++) {
            a[i + 1] = temp[i] + b[i];
        }
    }
}
