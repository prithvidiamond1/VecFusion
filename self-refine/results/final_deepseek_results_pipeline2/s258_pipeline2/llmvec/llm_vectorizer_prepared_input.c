void s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256]) {
    float s;
    for (int nl = 0; nl < iterations; nl++) {
        s = 0.;
        
        // First loop: compute s values and store them in a temporary array
        float s_vals[256];
        for (int i = 0; i < 256; ++i) {
            if (a[i] > 0.) {
                s = d[i] * d[i];
            }
            s_vals[i] = s;
        }
        
        // Second loop: compute b[i] using precomputed s_vals
        for (int i = 0; i < 256; ++i) {
            b[i] = s_vals[i] * c[i] + d[i];
        }
        
        // Third loop: compute e[i] using precomputed s_vals
        for (int i = 0; i < 256; ++i) {
            e[i] = (s_vals[i] + 1.0f) * aa[0][i];
        }
    }
}
