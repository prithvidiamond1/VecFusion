void s114_opt(int iterations, float aa[256][256], float bb[256][256])
{
    int loop_count = 200 * (iterations / 256);
    
    for (int nl = 0; nl < loop_count; nl++) {
        // Process all elements in parallel-friendly order
        for (int i = 1; i < 256; i++) {
            // Read all required upper triangle values first
            float upper_values[256];
            for (int j = 0; j < i; j++) {
                upper_values[j] = aa[j][i];
            }
            
            // Then compute and write all lower triangle values
            for (int j = 0; j < i; j++) {
                aa[i][j] = upper_values[j] + bb[i][j];
            }
        }
    }
}
