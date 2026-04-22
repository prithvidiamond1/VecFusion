void s242_opt(int iterations, int LEN_1D, float s1, float s2, float* a, float* b, float* c, float* d)
{
    float s12 = s1 + s2;
    for (int nl = 0; nl < iterations/5; nl++) {
        // Compute prefix sums of (b[i] + c[i] + d[i] + s12) independently
        // Then reconstruct a[i] = a[0] + sum_{k=1}^{i} (s12 + b[k] + c[k] + d[k])
        // Step 1: compute increments into a temporary scan array
        // We use b as a conceptual helper but keep a separate temp array approach
        // Instead: precompute cumulative sum in a temp buffer, then add a[0]
        float temp[LEN_1D];
        // Step 1: vectorizable - compute per-element increments
        for (int i = 1; i < LEN_1D; ++i) {
            temp[i] = s12 + b[i] + c[i] + d[i];
        }
        // Step 2: prefix sum (carry dependency isolated here)
        for (int i = 2; i < LEN_1D; ++i) {
            temp[i] = temp[i] + temp[i - 1];
        }
        // Step 3: vectorizable - write back results
        float a0 = a[0];
        for (int i = 1; i < LEN_1D; ++i) {
            a[i] = a0 + temp[i];
        }
    }
}
