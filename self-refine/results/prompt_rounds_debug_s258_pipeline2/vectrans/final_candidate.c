void s258_opt(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256]) {
    float s;
    for (int nl = 0; nl < iterations; nl++) {
        s = 0.;
        // First loop: compute s values with dependency
        float s_vals[256];
        for (int i = 0; i < 256; ++i) {
            // Remove branching to help vectorization
            float condition = a[i] > 0.0f ? 1.0f : 0.0f;
            s = condition * (d[i] * d[i]) + (1.0f - condition) * s;
            s_vals[i] = s;
        }
        
        // Second loop: compute b[i] using stored s values
        for (int i = 0; i < 256; ++i) {
            b[i] = s_vals[i] * c[i] + d[i];
        }
        
        // Third loop: compute e[i] using stored s values
        for (int i = 0; i < 256; ++i) {
            e[i] = (s_vals[i] + 1.0f) * aa[0][i];
        }
    }
}
