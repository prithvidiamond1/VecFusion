void s123_opt(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e) {
    int half_len = LEN_1D / 2;
    
    for (int nl = 0; nl < iterations; nl++) {
        // Phase 1: Compute all unconditional results
        for (int i = 0; i < half_len; i++) {
            float de = d[i] * e[i];
            a[2*i] = b[i] + de;  // Store at even positions
        }
        
        // Phase 2: Compute conditional results
        for (int i = 0; i < half_len; i++) {
            float de = d[i] * e[i];
            float cond_val = c[i] + de;
            int mask = c[i] > 0.0f;
            // Store conditional result at odd positions
            a[2*i + 1] = cond_val * mask + a[2*i + 1] * (1 - mask);
        }
        
        // Phase 3: Compact results
        int dst = 0;
        for (int i = 0; i < half_len; i++) {
            // Always write unconditional element
            a[dst] = a[2*i];
            dst++;
            
            // Write conditional element if valid
            if (a[2*i + 1] != a[2*i]) {  // Check if conditional was computed
                a[dst] = a[2*i + 1];
                dst++;
            }
        }
    }
}
