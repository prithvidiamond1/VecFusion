void s244_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d) {
    for (int nl = 0; nl < iterations; nl++) {
        // Store original a[i+1] values for i=0..LEN_1D-2
        float* a_next = (float*)__builtin_alloca(LEN_1D * sizeof(float));
        for (int i = 0; i < LEN_1D; ++i) {
            a_next[i] = a[i];
        }
        
        // Compute all b[i] updates first
        for (int i = 0; i < LEN_1D-1; ++i) {
            b[i] = c[i] + b[i];
        }
        
        // Compute a[i] = (b[i] - c[i]) + c[i] * d[i] for i=0..LEN_1D-2
        // This is equivalent to original: a[i] = b_old[i] + c[i] * d[i]
        for (int i = 0; i < LEN_1D-1; ++i) {
            a[i] = (b[i] - c[i]) + c[i] * d[i];
        }
        
        // Compute a[i+1] = b[i] + a_next[i+1] * d[i] for i=0..LEN_1D-2
        // This overwrites a[i+1] for i=0..LEN_1D-2, but note:
        // For i>0, a[i] was already set in previous loop and won't be touched here
        for (int i = 0; i < LEN_1D-1; ++i) {
            a[i+1] = b[i] + a_next[i+1] * d[i];
        }
    }
}
