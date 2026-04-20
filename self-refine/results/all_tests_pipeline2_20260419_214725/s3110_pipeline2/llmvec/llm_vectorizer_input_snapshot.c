float s3110(int iterations, float aa[256][256]) {
    int xindex, yindex;
    float max, chksum;
    int outer_iters = 100 * (iterations / 256);
    
    for (int nl = 0; nl < outer_iters; nl++) {
        max = aa[0][0];
        xindex = 0;
        yindex = 0;
        
        // First pass: find max value only (fully vectorizable)
        float row_max_arr[256];
        for (int i = 0; i < 256; i++) {
            float row_max = aa[i][0];
            for (int j = 1; j < 256; j++) {
                float val = aa[i][j];
                row_max = (val > row_max) ? val : row_max;
            }
            row_max_arr[i] = row_max;
        }
        
        // Reduce across rows to find global max
        for (int i = 0; i < 256; i++) {
            if (row_max_arr[i] > max) {
                max = row_max_arr[i];
            }
        }
        
        // Second pass: find last occurrence of max (vectorizable per row)
        // Process rows in reverse order to match original "last occurrence" semantics
        int found = 0;
        for (int i = 255; i >= 0 && !found; i--) {
            if (row_max_arr[i] == max) {
                // Scan this row from end to beginning for last occurrence
                for (int j = 255; j >= 0; j--) {
                    if (aa[i][j] == max) {
                        xindex = i;
                        yindex = j;
                        found = 1;
                        break;
                    }
                }
            }
        }
        
        chksum = max + (float)xindex + (float)yindex;
    }
    
    return max + xindex + 1 + yindex + 1;
}