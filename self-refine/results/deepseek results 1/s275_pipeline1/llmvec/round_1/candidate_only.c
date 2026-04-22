#include <string.h>

void vectorized_s275(int iterations, float aa[256][256], float bb[256][256], float cc[256][256])
{
    for (int nl = 0; nl < 10*(iterations/256); nl++) {
        for (int i = 0; i < 256; i++) {
            if (aa[0][i] > (float)0.) {
                // This loop has a loop-carried dependency through aa[j][i] = aa[j-1][i] + ...
                // so we cannot vectorize across j. We unroll manually.
                // Each iteration depends on the previous, so we just unroll 4x for ILP hints.
                int j = 1;
                for (; j < 253; j += 4) {
                    aa[j][i]   = aa[j-1][i]   + bb[j][i]   * cc[j][i];
                    aa[j+1][i] = aa[j][i]     + bb[j+1][i] * cc[j+1][i];
                    aa[j+2][i] = aa[j+1][i]   + bb[j+2][i] * cc[j+2][i];
                    aa[j+3][i] = aa[j+2][i]   + bb[j+3][i] * cc[j+3][i];
                }
                for (; j < 256; j++) {
                    aa[j][i] = aa[j-1][i] + bb[j][i] * cc[j][i];
                }
            }
        }
    }
}
