#include <stdint.h>

void vectorized_s231(int iterations, float aa[256][256], float bb[256][256])
{
    for (int nl = 0; nl < 100*(iterations/256); nl++) {
        // The inner loop over j has a loop-carried dependency through aa[j][i] = aa[j-1][i] + bb[j][i]
        // so we cannot vectorize over j. We vectorize over i instead (the middle loop).
        for (int j = 1; j < 256; j++) {
            int i = 0;
            // Vectorize over i in chunks of 8
            for (; i <= 256 - 8; i += 8) {
                aa[j][i+0] = aa[j-1][i+0] + bb[j][i+0];
                aa[j][i+1] = aa[j-1][i+1] + bb[j][i+1];
                aa[j][i+2] = aa[j-1][i+2] + bb[j][i+2];
                aa[j][i+3] = aa[j-1][i+3] + bb[j][i+3];
                aa[j][i+4] = aa[j-1][i+4] + bb[j][i+4];
                aa[j][i+5] = aa[j-1][i+5] + bb[j][i+5];
                aa[j][i+6] = aa[j-1][i+6] + bb[j][i+6];
                aa[j][i+7] = aa[j-1][i+7] + bb[j][i+7];
            }
            // Scalar cleanup
            for (; i < 256; i++) {
                aa[j][i] = aa[j-1][i] + bb[j][i];
            }
        }
    }
}
