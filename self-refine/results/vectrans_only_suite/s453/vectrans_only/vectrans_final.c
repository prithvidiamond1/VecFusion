
void s453_opt(int iterations,int LEN_1D, float* a, float *b) {
    float s;
    for (int nl = 0; nl < iterations*2; nl++) {
        s = 0.;
        // Split the loop: first compute s values, then compute a[i]
        float s_val = 0.;
        for (int i = 0; i < LEN_1D; i++) {
            s_val += (float)2.;
            // Store s_val in a temporary array? Not needed, we can compute directly
            // Instead, compute a[i] immediately after s update
            a[i] = s_val * b[i];
        }
    }
}
