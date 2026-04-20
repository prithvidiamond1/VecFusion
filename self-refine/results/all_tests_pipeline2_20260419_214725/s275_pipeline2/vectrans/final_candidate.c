void s275_opt(int iterations, float aa[256][256], float bb[256][256], float cc[256][256]) {
    int loop_count = 10 * (iterations / 256);
    
    for (int nl = 0; nl < loop_count; nl++) {
        // Precompute condition mask
        int update_col[256];
        for (int i = 0; i < 256; i++) {
            update_col[i] = (aa[0][i] > 0.0f);
        }
        
        // Loop interchange: process j outer, i inner to enable vectorization across i
        for (int j = 1; j < 256; j++) {
            // Vectorize across columns (i dimension)
            for (int i = 0; i < 256; i++) {
                if (update_col[i]) {
                    aa[j][i] = aa[j-1][i] + bb[j][i] * cc[j][i];
                }
            }
        }
    }
}
