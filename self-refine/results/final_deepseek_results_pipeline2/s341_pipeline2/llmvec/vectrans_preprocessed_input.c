void s341_opt(int iterations, int LEN_1D, float* a, float* b) {
    for (int nl = 0; nl < iterations; nl++) {
        int write_idx = 0;
        for (int i = 0; i < LEN_1D; i++) {
            float temp_b = b[i];
            int mask = temp_b > (float)0.;
            if (mask) {
                a[write_idx] = temp_b;
                write_idx += 1;
            }
        }
    }
}
