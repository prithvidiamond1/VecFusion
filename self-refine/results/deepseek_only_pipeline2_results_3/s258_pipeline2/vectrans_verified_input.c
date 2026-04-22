void s258(int iterations, float* a, float* b, float* c, float* d, float* e, float aa[256][256]) {
    float* aa_row = aa[0];
    
    for (int nl = 0; nl < iterations; nl++) {
        float s = 0.f;
        float s_vals[256];
        
        // First loop: compute s values with sequential dependency
        // Use instruction reordering: compute condition first, then update s
        for (int i = 0; i < 256; ++i) {
            float condition = a[i] > 0.f ? 1.f : 0.f;
            // Store current s before potential update
            float current_s = s;
            // Update s based on condition
            s = condition * (d[i] * d[i]) + (1.f - condition) * current_s;
            s_vals[i] = s;
        }
        
        // Second loop: compute b[i] - fully vectorizable
        for (int i = 0; i < 256; ++i) {
            b[i] = s_vals[i] * c[i] + d[i];
        }
        
        // Third loop: compute e[i] - fully vectorizable  
        for (int i = 0; i < 256; ++i) {
            e[i] = (s_vals[i] + 1.f) * aa_row[i];
        }
    }
}