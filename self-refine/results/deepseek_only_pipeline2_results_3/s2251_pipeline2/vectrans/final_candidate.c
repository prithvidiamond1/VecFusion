void s2251_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations; nl++) {
        float s = (float)0.0;
        
        // Precompute b[i] + c[i] for all i
        float* tmp = (float*)malloc(LEN_1D * sizeof(float));
        for (int i = 0; i < LEN_1D; i++) {
            tmp[i] = b[i] + c[i];
        }
        
        // First iteration
        a[0] = s * e[0];
        b[0] = a[0] + d[0];
        s = tmp[0];
        
        // Main loop - now s comes from precomputed tmp[i-1]
        for (int i = 1; i < LEN_1D; i++) {
            a[i] = s * e[i];
            b[i] = a[i] + d[i];
            s = tmp[i];
        }
        
        free(tmp);
    }
}
