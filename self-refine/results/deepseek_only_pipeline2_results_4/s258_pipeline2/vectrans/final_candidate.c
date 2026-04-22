void s258_opt(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256]) {
    float aa0[256];
    for (int i = 0; i < 256; ++i) {
        aa0[i] = aa[0][i];
    }
    
    for (int nl = 0; nl < iterations; nl++) {
        float temp_s[256];
        float d_sq[256];
        
        // Precompute d[i]*d[i] for all i
        for (int i = 0; i < 256; ++i) {
            d_sq[i] = d[i] * d[i];
        }
        
        float s = 0.0f;
        // First loop: compute s values with dependency
        // Use arithmetic masking to eliminate branching
        for (int i = 0; i < 256; ++i) {
            float mask = (a[i] > 0.0f) ? 1.0f : 0.0f;
            s = mask * d_sq[i] + (1.0f - mask) * s;
            temp_s[i] = s;
        }
        
        // Second loop: compute b[i] and e[i] using precomputed s values
        for (int i = 0; i < 256; ++i) {
            float s_val = temp_s[i];
            b[i] = s_val * c[i] + d[i];
            e[i] = (s_val + 1.0f) * aa0[i];
        }
    }
}
