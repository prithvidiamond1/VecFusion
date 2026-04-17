void s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256])
{
    float s_array[256];
    
    for (int nl = 0; nl < iterations; nl++) {
        float s = 0.0f;
        
        // First loop: compute all s values with dependency preserved
        for (int i = 0; i < 256; ++i) {
            if (a[i] > 0.0f) {
                s = d[i] * d[i];
            }
            s_array[i] = s;
        }
        
        // Second loop: compute b and e using precomputed s values
        // This loop has no dependencies and can be vectorized
        for (int i = 0; i < 256; ++i) {
            float s_val = s_array[i];
            b[i] = s_val * c[i] + d[i];
            e[i] = (s_val + 1.0f) * aa[0][i];
        }
    }
}