void s322_opt(int iterations, int LEN_1D, float* a, float* b, float* c) {
    for (int nl = 0; nl < iterations/2; nl++) {
        // Store original values to break loop-carried dependency
        float* orig_a = (float*)malloc(LEN_1D * sizeof(float));
        for (int i = 0; i < LEN_1D; i++) {
            orig_a[i] = a[i];
        }
        
        // Process first two elements separately
        if (LEN_1D > 1) {
            a[1] = orig_a[1] + orig_a[0] * b[1];
        }
        
        // Main loop - split into two independent computations
        for (int i = 2; i < LEN_1D; i++) {
            float temp = orig_a[i] + a[i-2] * c[i];
            a[i] = temp + a[i-1] * b[i];
        }
        
        free(orig_a);
    }
}
