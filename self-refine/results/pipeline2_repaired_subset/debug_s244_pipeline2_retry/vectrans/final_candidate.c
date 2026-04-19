void s244_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First loop: compute a[i] and update b[i]
        for (int i = 0; i < LEN_1D-1; ++i) {
            a[i] = b[i] + c[i] * d[i];
            b[i] = c[i] + b[i];
        }
        // Second loop: compute a[i+1] using updated b[i] and original a[i+1]
        for (int i = 0; i < LEN_1D-1; ++i) {
            float temp = a[i+1];
            a[i+1] = b[i] + temp * d[i];
        }
    }
}
