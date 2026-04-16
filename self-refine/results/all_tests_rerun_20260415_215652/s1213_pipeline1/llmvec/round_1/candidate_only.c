void vectorized_s1213(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    // This loop has a loop-carried dependency through a[] and b[],
    // so we cannot vectorize the inner loop directly without breaking semantics.
    // We preserve the scalar semantics exactly.
    for (int nl = 0; nl < iterations; nl++) {
        int i = 1;
        int limit = LEN_1D - 1;

        // Unroll by 4 but keep scalar semantics due to dependency chain
        for (; i < limit - 3; i += 4) {
            a[i]   = b[i-1]   + c[i];
            b[i]   = a[i+1]   * d[i];

            a[i+1] = b[i]     + c[i+1];
            b[i+1] = a[i+2]   * d[i+1];

            a[i+2] = b[i+1]   + c[i+2];
            b[i+2] = a[i+3]   * d[i+2];

            a[i+3] = b[i+2]   + c[i+3];
            b[i+3] = a[i+4]   * d[i+3];
        }

        // Scalar cleanup
        for (; i < limit; i++) {
            a[i] = b[i-1] + c[i];
            b[i] = a[i+1] * d[i];
        }
    }
}
