#include <string.h>

void vectorized_s323(int iterations, int LEN_1D, float* a, float *b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        float cd[LEN_1D];
        float ce[LEN_1D];

        // Vectorizable: compute cd and ce
        int i;
        for (i = 1; i + 3 < LEN_1D; i += 4) {
            cd[i]   = c[i]   * d[i];
            cd[i+1] = c[i+1] * d[i+1];
            cd[i+2] = c[i+2] * d[i+2];
            cd[i+3] = c[i+3] * d[i+3];
            ce[i]   = c[i]   * e[i];
            ce[i+1] = c[i+1] * e[i+1];
            ce[i+2] = c[i+2] * e[i+2];
            ce[i+3] = c[i+3] * e[i+3];
        }
        for (; i < LEN_1D; i++) {
            cd[i] = c[i] * d[i];
            ce[i] = c[i] * e[i];
        }

        // Build b_temp with increments
        float b_temp[LEN_1D];
        b_temp[0] = b[0];

        // Vectorizable: compute increments
        for (i = 1; i + 3 < LEN_1D; i += 4) {
            b_temp[i]   = cd[i]   + ce[i];
            b_temp[i+1] = cd[i+1] + ce[i+1];
            b_temp[i+2] = cd[i+2] + ce[i+2];
            b_temp[i+3] = cd[i+3] + ce[i+3];
        }
        for (; i < LEN_1D; i++) {
            b_temp[i] = cd[i] + ce[i];
        }

        // Serial prefix sum (recurrence, not vectorizable in general)
        for (i = 1; i < LEN_1D; i++) {
            b_temp[i] = b_temp[i-1] + b_temp[i];
        }

        // Vectorizable: compute a[i] = b_temp[i-1] + cd[i]
        for (i = 1; i + 3 < LEN_1D; i += 4) {
            a[i]   = b_temp[i-1] + cd[i];
            a[i+1] = b_temp[i]   + cd[i+1];
            a[i+2] = b_temp[i+1] + cd[i+2];
            a[i+3] = b_temp[i+2] + cd[i+3];
        }
        for (; i < LEN_1D; i++) {
            a[i] = b_temp[i-1] + cd[i];
        }

        // Vectorizable: copy b_temp back to b
        for (i = 1; i + 3 < LEN_1D; i += 4) {
            b[i]   = b_temp[i];
            b[i+1] = b_temp[i+1];
            b[i+2] = b_temp[i+2];
            b[i+3] = b_temp[i+3];
        }
        for (; i < LEN_1D; i++) {
            b[i] = b_temp[i];
        }
    }
}
