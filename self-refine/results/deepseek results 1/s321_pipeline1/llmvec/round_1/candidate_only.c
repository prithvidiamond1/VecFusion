void vectorized_s321(int iterations, int LEN_1D, float* a, float* b)
{
    // This loop has a loop-carried dependency: a[i] depends on a[i-1].
    // True vectorization is not possible without changing semantics.
    // We unroll the scalar loop to help the compiler while preserving correctness.
    for (int nl = 0; nl < iterations; nl++) {
        int i = 1;
        // Unroll by 4, but since there's a loop-carried dependency,
        // each iteration must complete before the next can start.
        // We unroll to hint the compiler but keep sequential semantics.
        for (; i <= LEN_1D - 4; i += 4) {
            a[i]   += a[i-1] * b[i];
            a[i+1] += a[i]   * b[i+1];
            a[i+2] += a[i+1] * b[i+2];
            a[i+3] += a[i+2] * b[i+3];
        }
        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            a[i] += a[i-1] * b[i];
        }
    }
}
