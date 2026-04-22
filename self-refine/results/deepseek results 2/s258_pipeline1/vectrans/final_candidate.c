void s258_opt(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256]) {
    for (int nl = 0; nl < iterations; nl++) {
        float s = 0.0f;
        float s_vals[256];
        
        // First loop: compute all s values with serial dependency
        for (int i = 0; i < 256; ++i) {
            s = (a[i] > 0.0f) ? (d[i] * d[i]) : s;
            s_vals[i] = s;
        }
        
        // Second loop: vectorizable computations using precomputed s_vals
        for (int i = 0; i < 256; ++i) {
            b[i] = s_vals[i] * c[i] + d[i];
            e[i] = (s_vals[i] + 1.0f) * aa[0][i];
        }
    }
}
