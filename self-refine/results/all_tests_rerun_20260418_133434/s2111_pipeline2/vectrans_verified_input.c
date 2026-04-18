void s2111(int iterations, float aa[256][256]) {
    int loop_count = 100 * (iterations / 256);
    
    for (int nl = 0; nl < loop_count; nl++) {
        // Process first row (j=1) separately
        float left_val = aa[1][0];
        for (int i = 1; i < 256; i++) {
            float new_val = (left_val + aa[0][i]) / 1.9f;
            left_val = new_val;
            aa[1][i] = new_val;
        }
        
        // Process remaining rows (j=2 to 255)
        for (int j = 2; j < 256; j++) {
            // Copy previous row to temporary array
            float prev_row[256];
            for (int k = 0; k < 256; k++) {
                prev_row[k] = aa[j-1][k];
            }
            
            // Process current row
            float left = aa[j][0];
            for (int i = 1; i < 256; i++) {
                float new_val = (left + prev_row[i]) / 1.9f;
                left = new_val;
                aa[j][i] = new_val;
            }
        }
    }
}