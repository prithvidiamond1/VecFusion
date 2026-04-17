void s261_opt(int iterations,int LEN_1D, float* a,float* b,float* c, float* d) {
    float t;
    for (int nl = 0; nl < iterations; nl++) {
        // First loop: compute t = a[i] + b[i] and store in a temporary array
        float temp_t[LEN_1D];
        for (int i = 1; i < LEN_1D; ++i) {
            temp_t[i] = a[i] + b[i];
        }
        
        // Second loop: compute t = c[i] * d[i] and update c[i]
        for (int i = 1; i < LEN_1D; ++i) {
            t = c[i] * d[i];
            c[i] = t;
        }
        
        // Third loop: update a[i] using temp_t and c[i-1]
        for (int i = 1; i < LEN_1D; ++i) {
            a[i] = temp_t[i] + c[i-1];
        }
    }
}
