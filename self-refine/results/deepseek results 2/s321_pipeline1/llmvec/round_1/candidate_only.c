void vectorized_s321(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < iterations; nl++) {
        int i = 1;
        // Unroll by 4, but must respect the carried dependency
        // Each iteration depends on previous a[i-1], so we unroll
        // sequentially to hint the compiler while keeping semantics
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
