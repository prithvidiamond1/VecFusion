float s13110(int iterations, float aa[256][256]) {
    int xindex, yindex;
    float max, chksum;
    int outer_loop_count = 100 * (iterations / 256);
    
    for (int nl = 0; nl < outer_loop_count; nl++) {
        // Step 1: Find per-row maxima and their first column indices
        float row_max[256];
        int row_max_col[256];
        
        for (int i = 0; i < 256; i++) {
            float rmax = aa[i][0];
            int rcol = 0;
            
            // Vectorizable: find first occurrence of max in each row
            for (int j = 1; j < 256; j++) {
                float current = aa[i][j];
                // Use > to get first occurrence (matches original)
                int is_greater = current > rmax;
                rmax = is_greater ? current : rmax;
                rcol = is_greater ? j : rcol;
            }
            
            row_max[i] = rmax;
            row_max_col[i] = rcol;
        }
        
        // Step 2: Find global maximum across rows and first row index
        max = row_max[0];
        int max_row = 0;
        
        // Vectorizable reduction for max value and first row index
        for (int i = 1; i < 256; i++) {
            float current = row_max[i];
            int is_greater = current > max;
            max = is_greater ? current : max;
            max_row = is_greater ? i : max_row;
        }
        
        // Step 3: For rows with same max value, find first occurrence in row-major order
        for (int i = 0; i < max_row; i++) {
            if (row_max[i] == max) {
                // Compare column indices to maintain first occurrence in row-major order
                if (row_max_col[i] < row_max_col[max_row]) {
                    max_row = i;
                }
            }
        }
        
        // Step 4: Get column index from pre-computed row_max_col
        xindex = max_row;
        yindex = row_max_col[max_row];
        
        chksum = max + (float)xindex + (float)yindex;
    }
    
    return max + (float)(xindex + 1) + (float)(yindex + 1);
}