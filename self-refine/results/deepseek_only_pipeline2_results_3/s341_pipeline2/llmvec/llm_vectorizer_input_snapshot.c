void s341(int iterations, int LEN_1D, float* a, float* b) {
    for (int nl = 0; nl < iterations; nl++) {
        int write_idx = 0;
        // Pre-calculate condition mask to separate dependency
        int cond_mask[LEN_1D];
        for (int i = 0; i < LEN_1D; i++) {
            cond_mask[i] = b[i] > 0.0f;
        }
        // Write loop with predictable access pattern
        for (int i = 0; i < LEN_1D; i++) {
            if (cond_mask[i]) {
                a[write_idx] = b[i];
                write_idx++;
            }
        }
    }
}