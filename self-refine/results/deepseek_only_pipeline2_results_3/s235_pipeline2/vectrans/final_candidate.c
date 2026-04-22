void s235_opt(int iterations, float* a, float* b, float* c, float aa[256][256], float bb[256][256])
{
    int loop_count = 200 * (iterations / 256);
    
    for (int nl = 0; nl < loop_count; nl++) {
        // 1. Update all a[i] first (fully vectorizable)
        for (int i = 0; i < 256; i++) {
            a[i] += b[i] * c[i];
        }
        
        // 2. Loop interchange: make j outer, i inner to enable vectorization across i
        // Process aa[0][i] as base for each column
        for (int j = 1; j < 256; j++) {
            // Vectorizable loop: no dependency across i
            for (int i = 0; i < 256; i++) {
                aa[j][i] = aa[j-1][i] + bb[j][i] * a[i];
            }
        }
    }
}
