void s161(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // Store original a[i] values before any updates
        float* a_orig = (float*)__builtin_alloca((LEN_1D-1) * sizeof(float));
        for (int i = 0; i < LEN_1D-1; ++i) {
            a_orig[i] = a[i];
        }
        
        // First pass: compute c[i+1] updates for b[i] < 0 cases
        for (int i = 0; i < LEN_1D-1; ++i) {
            if (b[i] < (float)0.) {
                c[i+1] = a_orig[i] + d[i] * d[i];
            }
        }
        
        // Second pass: compute a[i] updates for b[i] >= 0 cases
        // Uses potentially updated c[i] from previous iteration
        for (int i = 0; i < LEN_1D-1; ++i) {
            if (b[i] >= (float)0.) {
                a[i] = c[i] + d[i] * e[i];
            }
        }
    }
}