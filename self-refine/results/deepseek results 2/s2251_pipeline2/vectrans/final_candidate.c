void s2251_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e) {
    for (int nl = 0; nl < iterations; nl++) {
        // Precompute all s values to break the recurrence chain
        float s_prev = 0.0f;
        
        // Vectorizable loop for s_next precomputation
        for (int i = 0; i < LEN_1D; i++) {
            float s_next = b[i] + c[i];
            a[i] = s_prev * e[i];
            b[i] = a[i] + d[i];
            s_prev = s_next;
        }
    }
}
