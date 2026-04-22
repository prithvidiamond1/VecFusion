float s3110_opt(int iterations, float aa[256][256]) {
    int xindex, yindex;
    float max, chksum;
    int outer_loop_count = 100 * (iterations / 256);
    
    for (int nl = 0; nl < outer_loop_count; nl++) {
        max = aa[0][0];
        xindex = 0;
        yindex = 0;
        
        // Process each row independently
        for (int i = 0; i < 256; i++) {
            float row_max = aa[i][0];
            int row_yindex = 0;
            
            // Single pass: find maximum value and its last occurrence in row
            for (int j = 1; j < 256; j++) {
                float current = aa[i][j];
                // Update maximum and track last occurrence
                if (current >= row_max) {
                    row_max = current;
                    row_yindex = j;
                }
            }
            
            // Update global maximum with correct indices
            if (row_max > max) {
                max = row_max;
                xindex = i;
                yindex = row_yindex;
            }
        }
        
        chksum = max + (float)xindex + (float)yindex;
    }
    
    return max + xindex + 1 + yindex + 1;
}
