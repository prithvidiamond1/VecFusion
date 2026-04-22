void s161(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // Store original a and c values before modifications
        float* a_orig = (float*)__builtin_alloca(LEN_1D * sizeof(float));
        float* c_orig = (float*)__builtin_alloca(LEN_1D * sizeof(float));
        __builtin_memcpy(a_orig, a, LEN_1D * sizeof(float));
        __builtin_memcpy(c_orig, c, LEN_1D * sizeof(float));
        
        // First: compute all a[i] updates using original c[i]
        for (int i = 0; i < LEN_1D-1; ++i) {
            if (b[i] >= (float)0.) {
                a[i] = c_orig[i] + d[i] * e[i];
            }
        }
        
        // Second: compute all c[i+1] updates using updated a[i] values
        for (int i = 0; i < LEN_1D-1; ++i) {
            if (b[i] < (float)0.) {
                // Use a[i] which may have been updated in first loop
                c[i+1] = a[i] + d[i] * d[i];
            }
        }
    }
}
