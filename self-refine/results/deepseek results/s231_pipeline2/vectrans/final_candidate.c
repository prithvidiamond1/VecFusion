void s231_opt(int iterations, float aa[256][256], float bb[256][256]) {
    int loop_count = 100 * (iterations / 256);
    if (loop_count <= 0) return;
    
    for (int nl = 0; nl < loop_count; nl++) {
        for (int j = 1; j < 256; j++) {
            float* aa_prev = aa[j - 1];
            float* aa_curr = aa[j];
            float* bb_curr = bb[j];
            
            for (int i = 0; i < 256; i++) {
                aa_curr[i] = aa_prev[i] + bb_curr[i];
            }
        }
    }
}
