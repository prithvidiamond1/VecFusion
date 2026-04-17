void s256(int iterations, float* a, float* d, float aa[256][256], float bb[256][256]) {
    int outer_iters = 10 * (iterations / 256);
    
    for (int nl = 0; nl < outer_iters; nl++) {
        // Store initial a[0] value
        float a0 = a[0];
        float a_temp[256];
        
        // Split: compute all a_temp values first (vectorizable pattern)
        for (int j = 1; j < 256; j++) {
            a_temp[j] = (j & 1) ? (1.0f - a0) : a0;
        }
        
        // Loop reordering: make j outer, i inner for stride-1 access
        for (int j = 1; j < 256; j++) {
            float aj = a_temp[j];
            float dj = d[j];
            // Vectorizable inner loop with stride-1 access
            for (int i = 0; i < 256; i++) {
                aa[j][i] = aj + bb[j][i] * dj;
            }
        }
        
        // Update original a array (separate loop)
        for (int j = 1; j < 256; j++) {
            a[j] = a_temp[j];
        }
    }
}