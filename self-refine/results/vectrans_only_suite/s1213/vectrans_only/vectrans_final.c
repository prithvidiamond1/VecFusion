
void s1213_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d) {
    for (int nl = 0; nl < iterations; nl++) {
        // First, compute all b[i] using original a[i+1] values
        for (int i = 1; i < LEN_1D-1; i++) {
            b[i] = a[i+1] * d[i];
        }
        
        // Then compute a[i] using original b[i-1] values
        // Note: b[i-1] here is from previous nl iteration (unchanged in first loop)
        for (int i = 1; i < LEN_1D-1; i++) {
            a[i] = b[i-1] + c[i];
        }
    }
}
