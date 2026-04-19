void s244(int iterations, int LEN_1D, float* a, float* b, float* c, float* d) {
    for (int nl = 0; nl < iterations; nl++) {
        // Loop distribution: separate the independent computations
        // First, compute all b[i] updates
        for (int i = 0; i < LEN_1D - 1; ++i) {
            b[i] = c[i] + b[i];
        }
        
        // Then compute a[i] and a[i+1] using the updated b[i]
        // Handle first iteration separately to break dependency
        int i = 0;
        float a_next_temp = a[i+1];  // Save original a[1]
        a[i] = b[i] + c[i] * d[i];
        a[i+1] = b[i] + a_next_temp * d[i];
        
        // Main vectorizable loop for remaining iterations
        for (i = 1; i < LEN_1D - 1; ++i) {
            a_next_temp = a[i+1];  // Save original a[i+1]
            a[i] = b[i] + c[i] * d[i];
            a[i+1] = b[i] + a_next_temp * d[i];
        }
    }
}