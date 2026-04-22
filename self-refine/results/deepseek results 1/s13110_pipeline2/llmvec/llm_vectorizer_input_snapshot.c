float s13110(int iterations, float aa[256][256]) {
    int xindex, yindex;
    float max, chksum;
    int outer_iters = 100 * (iterations / 256);
    
    for (int nl = 0; nl < outer_iters; nl++) {
        max = aa[0][0];
        xindex = 0;
        yindex = 0;
        
        // Process each row independently
        for (int i = 0; i < 256; i++) {
            float local_max = aa[i][0];
            int local_yidx = 0;
            float* row_ptr = aa[i];
            
            // Single pass through row with direct comparison
            for (int j = 1; j < 256; j++) {
                float current = row_ptr[j];
                if (current > local_max) {
                    local_max = current;
                    local_yidx = j;
                }
            }
            
            // Compare row maximum with global maximum
            if (local_max > max) {
                max = local_max;
                xindex = i;
                yindex = local_yidx;
            } else if (local_max == max) {
                // Keep original semantics: only update if strictly greater
                // No update needed when equal
            }
        }
        
        chksum = max + (float)xindex + (float)yindex;
    }
    return max + (float)(xindex + 1) + (float)(yindex + 1);
}