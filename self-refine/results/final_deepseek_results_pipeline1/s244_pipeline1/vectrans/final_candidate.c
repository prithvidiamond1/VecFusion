void s244_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First part: compute b[i] and the new a[i] (no loop-carried dependency)
        for (int i = 0; i < LEN_1D - 1; ++i) {
            b[i] = c[i] + b[i];
        }
        
        // Second part: compute a[i+1] using updated b[i] and original a[i+1]
        // No loop-carried dependency since a[i+1] uses only b[i] and a[i+1] (same iteration)
        for (int i = 0; i < LEN_1D - 1; ++i) {
            a[i+1] = b[i] + a[i+1] * d[i];
        }
        
        // Third part: compute a[i] using b[i] and c[i] * d[i]
        // Independent from previous loops
        for (int i = 0; i < LEN_1D - 1; ++i) {
            a[i] = b[i] + c[i] * d[i];
        }
    }
}
