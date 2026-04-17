void vectorized_s322(int iterations, int LEN_1D, float* a, float *b, float* c)
{
    // This loop has a loop-carried dependency through a[i] (reads a[i-1] and a[i-2])
    // so true vectorization of the inner loop is not safe.
    // We provide an unrolled but semantically correct version.
    for (int nl = 0; nl < iterations/2; nl++) {
        int i = 2;
        // Unroll by 4, but since there's a loop-carried dependency,
        // we must process sequentially. Unroll manually for ILP hints.
        for (; i + 3 < LEN_1D; i += 4) {
            float a0 = a[i]   + a[i-1]   * b[i]   + a[i-2]   * c[i];
            a[i] = a0;
            float a1 = a[i+1] + a[i]     * b[i+1] + a[i-1]   * c[i+1];
            a[i+1] = a1;
            float a2 = a[i+2] + a[i+1]   * b[i+2] + a[i]     * c[i+2];
            a[i+2] = a2;
            float a3 = a[i+3] + a[i+2]   * b[i+3] + a[i+1]   * c[i+3];
            a[i+3] = a3;
        }
        // Scalar cleanup
        for (; i < LEN_1D; i++) {
            a[i] = a[i] + a[i-1] * b[i] + a[i-2] * c[i];
        }
    }
}
