float s13110(int iterations, float aa[256][256]) {
    int xindex, yindex;
    float max, chksum;
    for (int nl = 0; nl < 100 * (iterations / 256); nl++) {
        max = aa[0][0];
        xindex = 0;
        yindex = 0;
        
        // Process each row independently to find row maxima and their first occurrence
        float row_max[256];
        int row_xindex[256];
        int row_yindex[256];
        
        for (int i = 0; i < 256; i++) {
            float local_max = aa[i][0];
            int local_yindex = 0;
            
            // Vectorizable max-finding within row
            for (int j = 1; j < 256; j++) {
                float val = aa[i][j];
                if (val > local_max) {
                    local_max = val;
                    local_yindex = j;
                }
            }
            
            row_max[i] = local_max;
            row_xindex[i] = i;
            row_yindex[i] = local_yindex;
        }
        
        // Reduce across rows, preserving first occurrence in row-major order
        float global_max = row_max[0];
        int global_xindex = row_xindex[0];
        int global_yindex = row_yindex[0];
        
        for (int i = 1; i < 256; i++) {
            if (row_max[i] > global_max) {
                global_max = row_max[i];
                global_xindex = row_xindex[i];
                global_yindex = row_yindex[i];
            }
        }
        
        max = global_max;
        xindex = global_xindex;
        yindex = global_yindex;
        chksum = max + (float)xindex + (float)yindex;
    }
    return max + xindex + 1 + yindex + 1;
}