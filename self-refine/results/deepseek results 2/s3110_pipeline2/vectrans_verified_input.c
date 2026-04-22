float s3110(int iterations, float aa[256][256]) {
    int xindex, yindex;
    float max, chksum;
    int outer_loop_count = 100 * (iterations / 256);
    
    for (int nl = 0; nl < outer_loop_count; nl++) {
        // Initialize with first element
        max = aa[0][0];
        xindex = 0;
        yindex = 0;
        
        // Process each row independently
        for (int i = 0; i < 256; i++) {
            float row_max = aa[i][0];
            int row_yindex = 0;
            
            // First: find max value in row (vectorizable reduction)
            for (int j = 0; j < 256; j++) {
                float current = aa[i][j];
                row_max = (current > row_max) ? current : row_max;
            }
            
            // Second: find first index of row_max in this row
            // This preserves original semantics (first occurrence)
            for (int j = 0; j < 256; j++) {
                // Use conditional move pattern without branching
                int cond = (aa[i][j] == row_max) && (row_yindex == 0);
                row_yindex = cond ? j : row_yindex;
            }
            
            // Compare row max with global max
            int update_cond = (row_max > max);
            
            // Update global max if row_max > max
            max = update_cond ? row_max : max;
            xindex = update_cond ? i : xindex;
            yindex = update_cond ? row_yindex : yindex;
            
            // For equal values: update only if current position is earlier in row-major order
            int equal_cond = (row_max == max);
            int earlier_cond = (i < xindex) || ((i == xindex) && (row_yindex < yindex));
            
            // Update indices if equal and earlier in row-major order
            xindex = (equal_cond && earlier_cond) ? i : xindex;
            yindex = (equal_cond && earlier_cond) ? row_yindex : yindex;
        }
        
        chksum = max + (float)xindex + (float)yindex;
    }
    
    return max + xindex + 1 + yindex + 1;
}