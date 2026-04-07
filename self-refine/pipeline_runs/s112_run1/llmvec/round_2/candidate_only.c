void vectorized_s112(int *a, int *b, int iterations, int LEN_1D) {
    for (int nl = 0; nl < 3 * iterations; nl++) {
        int i = LEN_1D - 2;

        // Unroll by 8 - no loop-carried dependency since each iteration
        // writes a[i+1] and reads a[i], and i decreases, so no overlap
        for (; i >= 7; i -= 8) {
            a[i + 1] = a[i]     + b[i];
            a[i]     = a[i - 1] + b[i - 1];
            a[i - 1] = a[i - 2] + b[i - 2];
            a[i - 2] = a[i - 3] + b[i - 3];
            a[i - 3] = a[i - 4] + b[i - 4];
            a[i - 4] = a[i - 5] + b[i - 5];
            a[i - 5] = a[i - 6] + b[i - 6];
            a[i - 6] = a[i - 7] + b[i - 7];
        }

        // Scalar tail
        for (; i >= 0; i--) {
            a[i + 1] = a[i] + b[i];
        }
    }
}
