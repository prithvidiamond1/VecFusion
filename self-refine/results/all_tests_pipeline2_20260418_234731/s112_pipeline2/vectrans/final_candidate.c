void s112_opt(float *a, float *b, int iterations, int LEN_1D) {
    int total_outer = 3 * iterations;
    
    for (int nl = 0; nl < total_outer; nl++) {
        // Process in reverse order - no loop-carried dependency
        for (int i = LEN_1D - 2; i >= 0; i--) {
            float temp = a[i] + b[i];
            a[i + 1] = temp;
        }
    }
}
