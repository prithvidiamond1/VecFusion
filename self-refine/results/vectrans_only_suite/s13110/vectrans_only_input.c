
float s13110_opt(int iterations, float aa[256][256]) {
    int xindex, yindex;
    float max, chksum;
    int outer_iters = 100 * (iterations / 256);
    
    for (int nl = 0; nl < outer_iters; nl++) {
        float row_max[256];
        int row_yindex[256];
        
        // First pass: find max and its first occurrence in each row
        for (int i = 0; i < 256; i++) {
            float local_max = aa[i][0];
            int local_yindex = 0;
            
            // Vectorizable max reduction - find max value in row
            for (int j = 1; j < 256; j++) {
                float current_val = aa[i][j];
                if (current_val > local_max) {
                    local_max = current_val;
                }
            }
            
            // Find first occurrence of max value - restructured to avoid early break
            int found_idx = 0;
            for (int j = 0; j < 256; j++) {
                // Use conditional assignment without break for better vectorization
                int is_match = (aa[i][j] == local_max) && (found_idx == 0);
                found_idx = is_match ? j : found_idx;
            }
            
            row_max[i] = local_max;
            row_yindex[i] = found_idx;
        }
        
        // Second pass: find max across rows
        float final_max = row_max[0];
        
        // Vectorizable reduction to find global max value
        for (int i = 1; i < 256; i++) {
            if (row_max[i] > final_max) {
                final_max = row_max[i];
            }
        }
        
        // Find first row with global max - restructured to avoid early break
        int final_idx = 0;
        for (int i = 0; i < 256; i++) {
            // Use conditional assignment without break
            int is_match = (row_max[i] == final_max) && (final_idx == 0);
            final_idx = is_match ? i : final_idx;
        }
        
        max = final_max;
        xindex = final_idx;
        yindex = row_yindex[final_idx];
        chksum = max + (float)xindex + (float)yindex;
    }
    
    return max + xindex + 1 + yindex + 1;
}
