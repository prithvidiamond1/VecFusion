void s2111_opt(int iterations, float aa[256][256]) {
    int loop_bound = 100 * (iterations / 256);
    
    for (int nl = 0; nl < loop_bound; nl++) {
        // Process first row (j=1) separately
        float prev_val = aa[1][0];
        for (int i = 1; i < 256; i++) {
            float new_val = (prev_val + aa[0][i]) / 1.9f;
            aa[1][i] = new_val;
            prev_val = new_val;
        }
        
        // Process remaining rows (j=2 to 255)
        for (int j = 2; j < 256; j++) {
            // Copy previous row to break vertical dependency
            float prev_row[256];
            for (int i = 0; i < 256; i++) {
                prev_row[i] = aa[j-1][i];
            }
            
            // Process current row with only horizontal dependency
            float current_val = (aa[j][0] + prev_row[1]) / 1.9f;
            aa[j][1] = current_val;
            
            for (int i = 2; i < 256; i++) {
                current_val = (current_val + prev_row[i]) / 1.9f;
                aa[j][i] = current_val;
            }
        }
    }
}
