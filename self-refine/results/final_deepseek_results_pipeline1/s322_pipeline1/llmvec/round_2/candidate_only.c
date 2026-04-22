void vectorized_s322(int iterations, int LEN_1D, float* a, float *b, float *c) {
    int total_iters = iterations / 2;
    if (total_iters <= 0 || LEN_1D <= 2) return;
    
    for (int nl = 0; nl < total_iters; nl++) {
        // Scalar loop due to recurrence: a[i] depends on a[i-1] and a[i-2]
        for (int i = 2; i < LEN_1D; i++) {
            a[i] = a[i] + a[i - 1] * b[i] + a[i - 2] * c[i];
        }
    }
}
