void vectorized_s112(float *a, float *b, int iterations, int LEN_1D) {
    for (int nl = 0; nl < 3 * iterations; nl++) {
        int i = LEN_1D - 2;

        // Unroll by 4 - loop goes downward, each write a[i+1] reads a[i],
        // no overlap since indices decrease
        for (; i >= 3; i -= 4) {
            a[i + 1] = a[i]     + b[i];
            a[i]     = a[i - 1] + b[i - 1];
            a[i - 1] = a[i - 2] + b[i - 2];
            a[i - 2] = a[i - 3] + b[i - 3];
        }

        // Scalar tail
        for (; i >= 0; i--) {
            a[i + 1] = a[i] + b[i];
        }
    }
}
