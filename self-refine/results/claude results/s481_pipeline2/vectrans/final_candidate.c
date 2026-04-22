void s481_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    // Pre-scan d once to check if early exit would ever trigger
    // Since d is never modified, we only need to scan once
    for (int i = 0; i < LEN_1D; i++) {
        if (d[i] < (float)0.) {
            // Find where the early exit happens
            // Original: process nl=0 up to i, then return
            for (int j = 0; j < i; j++) {
                a[j] += b[j] * c[j];
            }
            return;
        }
    }

    // All d[i] >= 0, no early exit will occur in any iteration
    // Now the inner loop is fully vectorizable
    for (int nl = 0; nl < iterations; nl++) {
        for (int i = 0; i < LEN_1D; i++) {
            a[i] += b[i] * c[i];
        }
    }
}
