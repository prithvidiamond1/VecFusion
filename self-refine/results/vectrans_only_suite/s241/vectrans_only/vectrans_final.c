
void s241_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < 2*iterations; nl++) {
        // Store original a[i+1] values before they are overwritten
        float* a_old = (float*)__builtin_alloca(LEN_1D * sizeof(float));
        for (int i = 0; i < LEN_1D; i++) {
            a_old[i] = a[i];
        }
        
        // Compute all new a[i] values
        for (int i = 0; i < LEN_1D-1; i++) {
            a[i] = b[i] * c[i] * d[i];
        }
        
        // Compute b[i] using original a[i+1] values
        for (int i = 0; i < LEN_1D-1; i++) {
            b[i] = a[i] * a_old[i+1] * d[i];
        }
    }
}
