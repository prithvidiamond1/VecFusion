void s114_opt(int iterations, float aa[256][256], float bb[256][256])
{
    int loop_count = 200 * (iterations / 256);
    
    for (int nl = 0; nl < loop_count; nl++) {
        // Process by diagonal blocks to expose vectorization opportunities
        // Handle all j < i cases where aa[i][j] and aa[j][i] don't alias
        for (int i = 0; i < 256; i++) {
            // Vectorizable part: process in chunks where j and i are far enough apart
            // This ensures aa[i][j] and aa[j][i] are distinct memory locations
            int j;
            // Process with vector-friendly stride
            for (j = 0; j + 8 <= i; j += 8) {
                // Load transposed values first
                float temp0 = aa[j][i];
                float temp1 = aa[j+1][i];
                float temp2 = aa[j+2][i];
                float temp3 = aa[j+3][i];
                float temp4 = aa[j+4][i];
                float temp5 = aa[j+5][i];
                float temp6 = aa[j+6][i];
                float temp7 = aa[j+7][i];
                
                // Compute and store results
                aa[i][j] = temp0 + bb[i][j];
                aa[i][j+1] = temp1 + bb[i][j+1];
                aa[i][j+2] = temp2 + bb[i][j+2];
                aa[i][j+3] = temp3 + bb[i][j+3];
                aa[i][j+4] = temp4 + bb[i][j+4];
                aa[i][j+5] = temp5 + bb[i][j+5];
                aa[i][j+6] = temp6 + bb[i][j+6];
                aa[i][j+7] = temp7 + bb[i][j+7];
            }
            // Handle remaining elements
            for (; j < i; j++) {
                aa[i][j] = aa[j][i] + bb[i][j];
            }
        }
    }
}
