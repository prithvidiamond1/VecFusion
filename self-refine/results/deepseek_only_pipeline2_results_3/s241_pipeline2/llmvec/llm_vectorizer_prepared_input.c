void s241(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < 2*iterations; nl++) {
        // Store original a[i+1] values before modifying a[i]
        for (int i = 0; i < LEN_1D-1; i++) {
            float a_next = a[i+1];  // Store original a[i+1]
            a[i] = b[i] * c[i] * d[i];
            b[i] = a[i] * a_next * d[i];  // Use original a[i+1]
        }
    }
}