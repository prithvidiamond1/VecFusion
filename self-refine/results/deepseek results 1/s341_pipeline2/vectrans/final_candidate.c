void s341_opt(int iterations, int LEN_1D, float* a, float* b) {
    for (int nl = 0; nl < iterations; nl++) {
        int j = -1;
        // First loop: compute prefix sum of positive elements
        int prefix[LEN_1D];
        for (int i = 0; i < LEN_1D; i++) {
            prefix[i] = (b[i] > (float)0.) ? 1 : 0;
            if (i > 0) {
                prefix[i] += prefix[i-1];
            }
        }
        
        // Second loop: parallel copy using computed indices
        for (int i = 0; i < LEN_1D; i++) {
            if (b[i] > (float)0.) {
                a[prefix[i]-1] = b[i];
            }
        }
    }
}
