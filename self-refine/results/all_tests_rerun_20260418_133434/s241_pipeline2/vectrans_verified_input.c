void s241(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    float* a_orig = (float*)malloc(LEN_1D * sizeof(float));
    
    for (int nl = 0; nl < 2*iterations; nl++) {
        // Store original a values before they are overwritten
        for (int i = 0; i < LEN_1D; i++) {
            a_orig[i] = a[i];
        }
        
        // Compute new a[i] values
        for (int i = 0; i < LEN_1D-1; i++) {
            a[i] = b[i] * c[i] * d[i];
        }
        
        // Compute b[i] using original a[i+1] values
        for (int i = 0; i < LEN_1D-1; i++) {
            b[i] = a[i] * a_orig[i+1] * d[i];
        }
    }
    
    free(a_orig);
}