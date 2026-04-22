void s114(int iterations, float aa[256][256], float bb[256][256]) {
    int loop_count = 200 * (iterations / 256);
    
    for (int nl = 0; nl < loop_count; nl++) {
        // Process lower triangular part (j < i)
        for (int i = 1; i < 256; i++) {
            // Copy needed values from upper triangle before modification
            float temp_row[256];
            for (int k = 0; k < i; k++) {
                temp_row[k] = aa[k][i];
            }
            
            // Compute using temporary values (no dependency within this loop)
            for (int j = 0; j < i; j++) {
                aa[i][j] = temp_row[j] + bb[i][j];
            }
        }
    }
}