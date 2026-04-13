void vectorized_s112(int *a, int *b, int *out, int iterations) {
    for (int nl = 0; nl < 3 * iterations; nl++) {
        for (int i = out[0] - 2; i >= 0; i--) {
            a[i + 1] = a[i] + b[i];
        }
    }
}
