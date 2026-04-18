void vectorized_s222(int iterations, int LEN_1D, float*a, float*b, float*c, float* e)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // The e[i] recurrence has a backward dependency: e[i] = e[i-1] * e[i-1]
        // This cannot be vectorized due to the sequential dependency chain.
        for (int i = 1; i < LEN_1D; i++) {
            e[i] = e[i - 1] * e[i - 1];
        }
        // The a[i] loop has zero net effect and is omitted.
    }
}
