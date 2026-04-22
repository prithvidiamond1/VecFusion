void s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256]) {
    float s;
    for (int nl = 0; nl < iterations; nl++) {
        s = 0.;
        
        // First loop: compute s values serially
        float s_seq[256];
        for (int i = 0; i < 256; ++i) {
            if (a[i] > 0.) {
                s = d[i] * d[i];
            }
            s_seq[i] = s;
        }
        
        // Second loop: parallel computations using s_seq
        float* aa_row = aa[0];
        for (int i = 0; i < 256; ++i) {
            float s_val = s_seq[i];
            b[i] = s_val * c[i] + d[i];
            e[i] = (s_val + 1.0f) * aa_row[i];
        }
    }
}