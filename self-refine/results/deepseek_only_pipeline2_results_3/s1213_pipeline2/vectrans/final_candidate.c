void s1213_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // Store original a[i+1] values before they are overwritten
        for (int i = 1; i < LEN_1D-1; i++) {
            b[i] = a[i+1] * d[i];
        }
        // Now compute a[i] using b[i-1] from previous iteration
        for (int i = 1; i < LEN_1D-1; i++) {
            a[i] = b[i-1] + c[i];
        }
    }
}
