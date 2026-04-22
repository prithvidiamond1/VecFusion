void s482(int iterations, int LEN_1D, float* a, float *b, float *c) {
    for (int nl = 0; nl < iterations; nl++) {
        // First, find where the break condition occurs
        int break_idx = LEN_1D;
        #pragma clang loop vectorize(disable)
        for (int i = 0; i < LEN_1D; i++) {
            if (c[i] > b[i]) {
                break_idx = i;
                break;
            }
        }
        
        // Process all iterations before the break point (these can be vectorized)
        // Note: break_idx iteration is included because the break happens AFTER the update
        for (int i = 0; i <= break_idx && i < LEN_1D; i++) {
            a[i] += b[i] * c[i];
        }
    }
}
