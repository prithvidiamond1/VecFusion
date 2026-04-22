
void s112_opt(float *a, float *b, int iterations, int LEN_1D) {
    for (int nl = 0; nl < 3 * iterations; nl++) {
        // Create temporary array to break loop-carried dependency
        float temp[LEN_1D];
        
        // First element handled separately
        temp[0] = a[0];
        
        // Main computation in forward direction (vectorizable)
        for (int i = 0; i < LEN_1D - 1; i++) {
            temp[i + 1] = a[i] + b[i];
        }
        
        // Copy results back (vectorizable)
        for (int i = 1; i < LEN_1D; i++) {
            a[i] = temp[i];
        }
    }
}
