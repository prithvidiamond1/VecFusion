#include <stdint.h>
#include <string.h>

float vectorized_s13110(int iterations, float aa[256][256])
{
    int xindex, yindex;
    float max, chksum;

    for (int nl = 0; nl < 100 * (iterations / (256)); nl++) {
        max = aa[0][0];
        xindex = 0;
        yindex = 0;

        for (int i = 0; i < 256; i++) {
            // Process 8 floats at a time using unrolled scalar (vector-friendly)
            int j = 0;

            // Find local max in chunks of 8
            float lmax0 = aa[i][0], lmax1 = aa[i][1], lmax2 = aa[i][2], lmax3 = aa[i][3];
            float lmax4 = aa[i][4], lmax5 = aa[i][5], lmax6 = aa[i][6], lmax7 = aa[i][7];
            int lidx0 = 0, lidx1 = 1, lidx2 = 2, lidx3 = 3;
            int lidx4 = 4, lidx5 = 5, lidx6 = 6, lidx7 = 7;

            for (j = 8; j < 256; j += 8) {
                float v0 = aa[i][j+0], v1 = aa[i][j+1], v2 = aa[i][j+2], v3 = aa[i][j+3];
                float v4 = aa[i][j+4], v5 = aa[i][j+5], v6 = aa[i][j+6], v7 = aa[i][j+7];
                if (v0 > lmax0) { lmax0 = v0; lidx0 = j+0; }
                if (v1 > lmax1) { lmax1 = v1; lidx1 = j+1; }
                if (v2 > lmax2) { lmax2 = v2; lidx2 = j+2; }
                if (v3 > lmax3) { lmax3 = v3; lidx3 = j+3; }
                if (v4 > lmax4) { lmax4 = v4; lidx4 = j+4; }
                if (v5 > lmax5) { lmax5 = v5; lidx5 = j+5; }
                if (v6 > lmax6) { lmax6 = v6; lidx6 = j+6; }
                if (v7 > lmax7) { lmax7 = v7; lidx7 = j+7; }
            }

            // Reduce the 8 lanes
            float row_max = lmax0; int row_idx = lidx0;
            if (lmax1 > row_max) { row_max = lmax1; row_idx = lidx1; }
            if (lmax2 > row_max) { row_max = lmax2; row_idx = lidx2; }
            if (lmax3 > row_max) { row_max = lmax3; row_idx = lidx3; }
            if (lmax4 > row_max) { row_max = lmax4; row_idx = lidx4; }
            if (lmax5 > row_max) { row_max = lmax5; row_idx = lidx5; }
            if (lmax6 > row_max) { row_max = lmax6; row_idx = lidx6; }
            if (lmax7 > row_max) { row_max = lmax7; row_idx = lidx7; }

            if (row_max > max) {
                max = row_max;
                xindex = i;
                yindex = row_idx;
            }
        }

        chksum = max + (float)xindex + (float)yindex;
    }

    return max + xindex + 1 + yindex + 1;
}
