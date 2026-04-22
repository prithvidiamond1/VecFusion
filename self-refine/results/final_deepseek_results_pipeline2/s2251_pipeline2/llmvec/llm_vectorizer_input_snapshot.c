void s2251_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations; nl++) {
        float s = (float)0.0;
        
        // First: compute all s_vals = b[i] + c[i] (vectorizable)
        float* s_vals = (float*)malloc(LEN_1D * sizeof(float));
        for (int i = 0; i < LEN_1D; i++) {
            s_vals[i] = b[i] + c[i];
        }
        
        // Second: compute a[i] = s_vals[i-1] * e[i] for i>=1 (vectorizable)
        a[0] = s * e[0];  // i=0 case
        for (int i = 1; i < LEN_1D; i++) {
            a[i] = s_vals[i-1] * e[i];
        }
        s = s_vals[LEN_1D-1];  // final s value
        
        // Third: update b[i] = a[i] + d[i] (vectorizable)
        for (int i = 0; i < LEN_1D; i++) {
            b[i] = a[i] + d[i];
        }
        
        free(s_vals);
    }
}
