#include <stdint.h>

void vectorized_s233(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    for (int nl = 0; nl < 100*(iterations/256); nl++) {
        for (int i = 1; i < 256; i++) {
            // First loop: aa[j][i] = aa[j-1][i] + cc[j][i], j from 1 to 255
            // This is a sequential dependency on j, so we process scalar
            for (int j = 1; j < 256; j++) {
                aa[j][i] = aa[j-1][i] + cc[j][i];
            }

            // Second loop: bb[j][i] = bb[j][i-1] + cc[j][i], j from 1 to 255
            // No dependency on j, so we can vectorize over j
            // Process in chunks of 8
            int j = 1;
            for (; j <= 255 - 7; j += 8) {
                bb[j+0][i] = bb[j+0][i-1] + cc[j+0][i];
                bb[j+1][i] = bb[j+1][i-1] + cc[j+1][i];
                bb[j+2][i] = bb[j+2][i-1] + cc[j+2][i];
                bb[j+3][i] = bb[j+3][i-1] + cc[j+3][i];
                bb[j+4][i] = bb[j+4][i-1] + cc[j+4][i];
                bb[j+5][i] = bb[j+5][i-1] + cc[j+5][i];
                bb[j+6][i] = bb[j+6][i-1] + cc[j+6][i];
                bb[j+7][i] = bb[j+7][i-1] + cc[j+7][i];
            }
            // Scalar cleanup
            for (; j < 256; j++) {
                bb[j][i] = bb[j][i-1] + cc[j][i];
            }
        }
    }
}
