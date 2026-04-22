float s3110(int iterations, float aa[256][256]) {
    int xindex, yindex;
    float max, chksum;
    
    for (int nl = 0; nl < 100 * (iterations / 256); nl++) {
        // Initialize with first element
        max = aa[0][0];
        xindex = 0;
        yindex = 0;
        
        // First pass: find global maximum value (vectorizable reduction)
        for (int i = 0; i < 256; i++) {
            for (int j = 0; j < 256; j++) {
                float val = aa[i][j];
                int cond = val > max;
                max = cond ? val : max;
            }
        }
        
        // Second pass: find first occurrence of max in row-major order
        // This can be vectorized with early exit optimization
        int found = 0;
        for (int i = 0; i < 256 && !found; i++) {
            for (int j = 0; j < 256 && !found; j++) {
                int is_max = (aa[i][j] == max);
                found = is_max;
                xindex = is_max ? i : xindex;
                yindex = is_max ? j : yindex;
            }
        }
        
        chksum = max + (float)xindex + (float)yindex;
    }
    
    return max + xindex + 1 + yindex + 1;
}