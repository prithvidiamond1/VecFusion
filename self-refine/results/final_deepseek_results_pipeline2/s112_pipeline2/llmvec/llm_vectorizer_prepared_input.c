void s112(float *a, float *b, int iterations, int LEN_1D) {
    int total_iters = 3 * iterations;
    
    for (int nl = 0; nl < total_iters; nl++) {
        // Process in backward direction to preserve dependency
        // No loop splitting needed as each iteration is independent in backward order
        for (int i = LEN_1D - 2; i >= 0; i--) {
            float temp = a[i] + b[i];
            a[i + 1] = temp;
        }
    }
}
