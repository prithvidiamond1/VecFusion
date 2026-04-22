#include <stdint.h>
#include <float.h>

void vectorized_s3110(float aa[][256], int i, float *out_row_max, int *out_row_yindex) {
    float row_max = aa[i][0];
    int row_yindex = 0;

    // Process with manual unrolling (4-wide) starting from j=1
    int j = 1;

    // Scalar cleanup for the first few elements until we can do unrolled blocks
    // We'll do 4-wide unrolling
    for (; j <= 252; j += 4) {
        float c0 = aa[i][j];
        float c1 = aa[i][j+1];
        float c2 = aa[i][j+2];
        float c3 = aa[i][j+3];

        if (c0 > row_max) { row_max = c0; row_yindex = j; }
        if (c1 > row_max) { row_max = c1; row_yindex = j+1; }
        if (c2 > row_max) { row_max = c2; row_yindex = j+2; }
        if (c3 > row_max) { row_max = c3; row_yindex = j+3; }
    }

    // Scalar tail
    for (; j < 256; j++) {
        float current = aa[i][j];
        if (current > row_max) {
            row_max = current;
            row_yindex = j;
        }
    }

    *out_row_max = row_max;
    *out_row_yindex = row_yindex;
}
