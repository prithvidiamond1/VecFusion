void s322(int iterations, int LEN_1D, float* a, float* b, float* c) {
    for (int nl = 0; nl < iterations/2; nl++) {
        // Store original values to break loop-carried dependency
        float prev2 = a[0];
        float prev1 = a[1];
        
        // Main computation loop with scalar replacement
        for (int i = 2; i < LEN_1D; i++) {
            float current = a[i];
            float temp = current + prev1 * b[i] + prev2 * c[i];
            prev2 = prev1;
            prev1 = temp;
            a[i] = temp;
        }
    }
}
