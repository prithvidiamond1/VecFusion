float s13110(int iterations, float aa[256][256]) {
    int xindex, yindex;
    float max, chksum;
    
    for (int nl = 0; nl < 100 * (iterations / 256); nl++) {
        max = aa[0][0];
        xindex = 0;
        yindex = 0;
        
        float row_max[256];
        int row_yindex[256];
        
        // First: compute row maxima and their column indices
        for (int i = 0; i < 256; i++) {
            float local_max = aa[i][0];
            int local_y = 0;
            
            // Process all columns with branchless updates
            for (int j = 1; j < 256; j++) {
                float val = aa[i][j];
                int cond = val > local_max;
                local_max = cond ? val : local_max;
                local_y = cond ? j : local_y;
            }
            row_max[i] = local_max;
            row_yindex[i] = local_y;
        }
        
        // Second: find global maximum across rows
        for (int i = 0; i < 256; i++) {
            float val = row_max[i];
            int cond = val > max;
            max = cond ? val : max;
            xindex = cond ? i : xindex;
            yindex = cond ? row_yindex[i] : yindex;
        }
        
        chksum = max + (float)xindex + (float)yindex;
    }
    return max + xindex + 1 + yindex + 1;
}