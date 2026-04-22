float s13110(int iterations, float aa[256][256]) {
    int xindex, yindex;
    float max, chksum;
    int outer_loop_count = 100 * (iterations / 256);
    
    for (int nl = 0; nl < outer_loop_count; nl++) {
        // First pass: find row-wise max values (vectorizable)
        float row_max_vals[256];
        for (int i = 0; i < 256; i++) {
            float row_max = aa[i][0];
            for (int j = 1; j < 256; j++) {
                float current = aa[i][j];
                if (current > row_max) {
                    row_max = current;
                }
            }
            row_max_vals[i] = row_max;
        }
        
        // Second pass: find global max value across rows (vectorizable)
        max = row_max_vals[0];
        for (int i = 1; i < 256; i++) {
            if (row_max_vals[i] > max) {
                max = row_max_vals[i];
            }
        }
        
        // Third pass: find first occurrence of global max (row-major order)
        xindex = 0;
        yindex = 0;
        int found = 0;
        for (int i = 0; i < 256 && !found; i++) {
            if (row_max_vals[i] == max) {
                for (int j = 0; j < 256 && !found; j++) {
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
    
    return max + (float)(xindex + 1) + (float)(yindex + 1);
}
