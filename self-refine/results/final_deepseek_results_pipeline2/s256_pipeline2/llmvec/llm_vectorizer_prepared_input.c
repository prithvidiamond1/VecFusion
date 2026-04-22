void s256(int iterations, float* a, float* d, float aa[256][256], float bb[256][256]) {
    int outer_iters = 10 * (iterations / 256);
    float a_prev[256];
    
    for (int nl = 0; nl < outer_iters; nl++) {
        // Store initial a values for this nl iteration
        for (int k = 0; k < 256; k++) {
            a_prev[k] = a[k];
        }
        
        // Process all i for each j to enable vectorization over i
        for (int j = 1; j < 256; j++) {
            // Compute a[j] once per j (independent of i)
            a[j] = (float)1.0 - a[j - 1];
            
            // Vectorizable loop over i
            for (int i = 0; i < 256; i++) {
                aa[j][i] = a[j] + bb[j][i] * d[j];
            }
        }
        
        // Restore a[0] for next iteration
        a[0] = a_prev[0];
    }
}
