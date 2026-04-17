void s261_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d) {
    float t;
    for (int nl = 0; nl < iterations; nl++) {
        // First, update c[i] = c[i] * d[i] for all i
        // so that when we compute a[i] = a[i]+b[i]+c[i-1],
        // c[i-1] is already the updated value.
        for (int i = 1; i < LEN_1D; ++i) {
            c[i] = c[i] * d[i];
        }
        // Now compute a[i] using updated c[i-1]
        for (int i = 1; i < LEN_1D; ++i) {
            a[i] = a[i] + b[i] + c[i-1];
        }
    }
}
