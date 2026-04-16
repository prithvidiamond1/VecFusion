float s13110(int iterations, float aa[256][256])
{
    int xindex, yindex;
    float max, chksum;
    for (int nl = 0; nl < 100*(iterations/(256)); nl++) {
        max = aa[0][0];
        xindex = 0;
        yindex = 0;

        float local_max[256];
        int local_yindex[256];

        // Initialize local arrays
        for (int i = 0; i < 256; i++) {
            local_max[i] = aa[i][0];
            local_yindex[i] = 0;
        }

        // Inner loop: find max value per row (pure value reduction, vectorizable)
        for (int i = 0; i < 256; i++) {
            for (int j = 1; j < 256; j++) {
                int cond = aa[i][j] > local_max[i];
                local_max[i] = cond * aa[i][j] + (1 - cond) * local_max[i];
            }
        }

        // Find max index per row separately (split from value reduction)
        for (int i = 0; i < 256; i++) {
            for (int j = 1; j < 256; j++) {
                int cond = aa[i][j] == local_max[i];
                local_yindex[i] = cond * j + (1 - cond) * local_yindex[i];
            }
        }

        // Find global max value (vectorizable reduction)
        float global_max = local_max[0];
        for (int i = 1; i < 256; i++) {
            global_max = local_max[i] > global_max ? local_max[i] : global_max;
        }

        // Find xindex: first scan for matching row (split into single variable update)
        for (int i = 0; i < 256; i++) {
            int cond = local_max[i] == global_max;
            xindex = cond * i + (1 - cond) * xindex;
        }

        // Find yindex from xindex directly (no loop needed)
        yindex = local_yindex[xindex];

        max = global_max;

        chksum = max + (float)xindex + (float)yindex;
    }
    return max + xindex + 1 + yindex + 1;
}