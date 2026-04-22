void s482(int iterations, int LEN_1D, float* a, float *b, float *c) {
    // Hoist invariant break detection outside nl loop
    int break_index = LEN_1D;
    for (int i = 0; i < LEN_1D; i++) {
        if (c[i] > b[i]) {
            break_index = i + 1; // Include current iteration
            break;
        }
    }
    
    // Main arithmetic loop with vectorizable structure
    for (int nl = 0; nl < iterations; nl++) {
        for (int i = 0; i < break_index; i++) {
            a[i] += b[i] * c[i];
        }
    }
}