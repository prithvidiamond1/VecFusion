void vectorized_s281(int iterations, int LEN_1D, float* a, float* b, float* c)
{
    float x0, x1, x2, x3;
    for (int nl = 0; nl < iterations; nl++) {
        int i = 0;
        // Process in chunks of 4, but only where there's no read-after-write dependency
        // Dependency occurs when LEN_1D-i-1 < i, i.e., i > (LEN_1D-1)/2
        // Safe to vectorize only when all 4 read indices > all 4 write indices
        // i.e., LEN_1D-i-4 > i+3, i.e., i < (LEN_1D-4)/2
        int safe_limit = (LEN_1D - 4) / 2;
        if (safe_limit < 0) safe_limit = 0;
        // Align safe_limit to multiple of 4
        safe_limit = (safe_limit / 4) * 4;

        for (i = 0; i < safe_limit; i += 4) {
            x0 = a[LEN_1D-i-1] + b[i]   * c[i];
            x1 = a[LEN_1D-i-2] + b[i+1] * c[i+1];
            x2 = a[LEN_1D-i-3] + b[i+2] * c[i+2];
            x3 = a[LEN_1D-i-4] + b[i+3] * c[i+3];
            a[i]   = x0 - 1.0f;
            a[i+1] = x1 - 1.0f;
            a[i+2] = x2 - 1.0f;
            a[i+3] = x3 - 1.0f;
            b[i]   = x0;
            b[i+1] = x1;
            b[i+2] = x2;
            b[i+3] = x3;
        }

        // Scalar cleanup for the rest (where dependencies may exist)
        for (; i < LEN_1D; i++) {
            float x = a[LEN_1D-i-1] + b[i] * c[i];
            a[i] = x - 1.0f;
            b[i] = x;
        }
    }
}
