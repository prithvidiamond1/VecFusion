void s451(int iterations, int LEN_1D, float* a, float* b, float* c) {
    int loop_count = iterations / 5;
    
    for (int nl = 0; nl < loop_count; nl++) {
        // Single vectorizable loop - no dependencies between iterations
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = sinf(b[i]) + cosf(c[i]);
        }
    }
}