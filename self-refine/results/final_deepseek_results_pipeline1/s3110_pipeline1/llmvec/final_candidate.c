typedef float float256 __attribute__((ext_vector_type(256)));

float vectorized_s3110(int iterations, float aa[256][256]) {
    int xindex, yindex;
    float max, chksum;
    int nl_limit = 100 * (iterations / 256);
    for (int nl = 0; nl < nl_limit; nl++) {
        max = aa[0][0];
        xindex = 0;
        yindex = 0;
        for (int i = 0; i < 256; i++) {
            float row_max = aa[i][0];
            int row_x = i;
            int row_y = 0;
            for (int j = 1; j < 256; j++) {
                if (aa[i][j] > row_max) {
                    row_max = aa[i][j];
                    row_y = j;
                }
            }
            if (row_max > max) {
                max = row_max;
                xindex = row_x;
                yindex = row_y;
            }
        }
        chksum = max + (float)xindex + (float)yindex;
    }
    return max + xindex + 1 + yindex + 1;
}
