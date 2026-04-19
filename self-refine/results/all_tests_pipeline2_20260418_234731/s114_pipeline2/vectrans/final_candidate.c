void s114_opt(int iterations, float aa[256][256], float bb[256][256]) {
    int loop_count = 200 * (iterations / 256);
    
    for (int nl = 0; nl < loop_count; nl++) {
        // Temporary array to store computed values for lower triangle
        float temp_lower[256][256];
        
        // First compute all new values into temporary array
        // This breaks the perceived dependency since we only read from aa
        for (int i = 0; i < 256; i++) {
            int j = 0;
            // Process in chunks of 4 for potential vectorization
            for (; j + 3 < i; j += 4) {
                temp_lower[i][j] = aa[j][i] + bb[i][j];
                temp_lower[i][j+1] = aa[j+1][i] + bb[i][j+1];
                temp_lower[i][j+2] = aa[j+2][i] + bb[i][j+2];
                temp_lower[i][j+3] = aa[j+3][i] + bb[i][j+3];
            }
            // Handle remaining elements
            for (; j < i; j++) {
                temp_lower[i][j] = aa[j][i] + bb[i][j];
            }
        }
        
        // Then copy back to original array
        // This is a simple contiguous memory copy that can be vectorized
        for (int i = 0; i < 256; i++) {
            int j = 0;
            // Process in chunks of 4 for potential vectorization
            for (; j + 3 < i; j += 4) {
                aa[i][j] = temp_lower[i][j];
                aa[i][j+1] = temp_lower[i][j+1];
                aa[i][j+2] = temp_lower[i][j+2];
                aa[i][j+3] = temp_lower[i][j+3];
            }
            // Handle remaining elements
            for (; j < i; j++) {
                aa[i][j] = temp_lower[i][j];
            }
        }
    }
}
