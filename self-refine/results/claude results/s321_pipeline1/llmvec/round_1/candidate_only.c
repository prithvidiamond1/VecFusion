void vectorized_s321(int iterations, int LEN_1D, float* a, float* b)
{
    // This loop has a loop-carried dependency: a[i] depends on a[i-1].
    // True vectorization is not possible without changing semantics.
    // We preserve exact scalar semantics with an unrolled scalar loop.
    for (int nl = 0; nl < iterations; nl++) {
        int i = 1;
        // Unroll by 4 — still sequential due to dependency, but compiler-friendly
        for (; i + 3 < LEN_1D; i += 4) {
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
