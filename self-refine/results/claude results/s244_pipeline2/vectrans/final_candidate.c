void s244_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    float* orig_a = (float*)malloc(LEN_1D * sizeof(float));

    for (int nl = 0; nl < iterations; nl++) {
        // Store original a values before any updates
        memcpy(orig_a, a, LEN_1D * sizeof(float));

        // Loop 1: compute new b[i] = c[i] + b[i] (original b[i])
        for (int i = 0; i < LEN_1D - 1; ++i) {
            b[i] = c[i] + b[i];
        }

        // Loop 2: compute a[i+1] = new_b[i] + orig_a[i+1] * d[i]
        // Must run before Loop 3, since Loop 3 overwrites a[i] which is a[i+1] for previous i
        for (int i = 0; i < LEN_1D - 1; ++i) {
            a[i + 1] = b[i] + orig_a[i + 1] * d[i];
        }

        // Loop 3: compute a[i] = orig_b[i] + c[i]*d[i]
        // orig_b[i] = b[i] - c[i], since b[i] is already updated
        // This overwrites a[i] set by Loop 2 at index i (from i-1 iteration), which is correct
        for (int i = 0; i < LEN_1D - 1; ++i) {
            a[i] = (b[i] - c[i]) + c[i] * d[i];
        }
    }

    free(orig_a);
}
