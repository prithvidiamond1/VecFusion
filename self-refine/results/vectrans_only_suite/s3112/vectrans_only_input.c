
float s3112_opt(int iterations, int LEN_1D, float* a, float* b) {
    float sum = 0.0f;
    for (int nl = 0; nl < iterations; nl++) {
        float accum = 0.0f;
        
        // First loop: pure reduction to compute total sum
        for (int i = 0; i < LEN_1D; i++) {
            accum += a[i];
        }
        
        // Second loop: compute prefix sums using vectorizable pattern
        // Start with first element
        if (LEN_1D > 0) {
            b[0] = a[0];
        }
        
        // Main vectorizable loop for remaining elements
        for (int i = 1; i < LEN_1D; i++) {
            b[i] = b[i-1] + a[i];
        }
        
        sum = accum;
    }
    return sum;
}
