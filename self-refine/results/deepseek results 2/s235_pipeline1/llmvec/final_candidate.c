#include <stdint.h>

void vectorized_s235(int iterations, float* a, float* b, float* c, float aa[256][256], float bb[256][256])
{
    int limit = 200 * (iterations / 256);

    for (int nl = 0; nl < limit; nl++) {
        // Vectorize the outer i loop for the a[i] update
        int i = 0;
        for (; i <= 256 - 8; i += 8) {
            a[i+0] += b[i+0] * c[i+0];
            a[i+1] += b[i+1] * c[i+1];
            a[i+2] += b[i+2] * c[i+2];
            a[i+3] += b[i+3] * c[i+3];
            a[i+4] += b[i+4] * c[i+4];
            a[i+5] += b[i+5] * c[i+5];
            a[i+6] += b[i+6] * c[i+6];
            a[i+7] += b[i+7] * c[i+7];
        }
        for (; i < 256; i++) {
            a[i] += b[i] * c[i];
        }

        // The inner j loop has a loop-carried dependency on aa[j-1][i],
        // so we vectorize across i for each fixed j.
        for (int j = 1; j < 256; j++) {
            int ii = 0;
            for (; ii <= 256 - 8; ii += 8) {
                aa[j][ii+0] = aa[j-1][ii+0] + bb[j][ii+0] * a[ii+0];
                aa[j][ii+1] = aa[j-1][ii+1] + bb[j][ii+1] * a[ii+1];
                aa[j][ii+2] = aa[j-1][ii+2] + bb[j][ii+2] * a[ii+2];
                aa[j][ii+3] = aa[j-1][ii+3] + bb[j][ii+3] * a[ii+3];
                aa[j][ii+4] = aa[j-1][ii+4] + bb[j][ii+4] * a[ii+4];
                aa[j][ii+5] = aa[j-1][ii+5] + bb[j][ii+5] * a[ii+5];
                aa[j][ii+6] = aa[j-1][ii+6] + bb[j][ii+6] * a[ii+6];
                aa[j][ii+7] = aa[j-1][ii+7] + bb[j][ii+7] * a[ii+7];
            }
            for (; ii < 256; ii++) {
                aa[j][ii] = aa[j-1][ii] + bb[j][ii] * a[ii];
            }
        }
    }
}
