#include <stdlib.h>

void vectorized_s241(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    float* a_orig = (float*)malloc(LEN_1D * sizeof(float));

    for (int nl = 0; nl < 2 * iterations; nl++) {
        // Store original a values before they are overwritten
        int i = 0;
        for (; i <= LEN_1D - 8; i += 8) {
            a_orig[i+0] = a[i+0];
            a_orig[i+1] = a[i+1];
            a_orig[i+2] = a[i+2];
            a_orig[i+3] = a[i+3];
            a_orig[i+4] = a[i+4];
            a_orig[i+5] = a[i+5];
            a_orig[i+6] = a[i+6];
            a_orig[i+7] = a[i+7];
        }
        for (; i < LEN_1D; i++) {
            a_orig[i] = a[i];
        }

        // Compute new a[i] values for i in [0, LEN_1D-2]
        // Unrolled loop: i+7 <= LEN_1D-2, so i <= LEN_1D-9
        i = 0;
        for (; i <= LEN_1D - 9; i += 8) {
            a[i+0] = b[i+0] * c[i+0] * d[i+0];
            a[i+1] = b[i+1] * c[i+1] * d[i+1];
            a[i+2] = b[i+2] * c[i+2] * d[i+2];
            a[i+3] = b[i+3] * c[i+3] * d[i+3];
            a[i+4] = b[i+4] * c[i+4] * d[i+4];
            a[i+5] = b[i+5] * c[i+5] * d[i+5];
            a[i+6] = b[i+6] * c[i+6] * d[i+6];
            a[i+7] = b[i+7] * c[i+7] * d[i+7];
        }
        for (; i < LEN_1D - 1; i++) {
            a[i] = b[i] * c[i] * d[i];
        }

        // Compute b[i] using new a[i] and original a[i+1] for i in [0, LEN_1D-2]
        // Unrolled loop: i+7 <= LEN_1D-2, so i <= LEN_1D-9
        i = 0;
        for (; i <= LEN_1D - 9; i += 8) {
            b[i+0] = a[i+0] * a_orig[i+1] * d[i+0];
            b[i+1] = a[i+1] * a_orig[i+2] * d[i+1];
            b[i+2] = a[i+2] * a_orig[i+3] * d[i+2];
            b[i+3] = a[i+3] * a_orig[i+4] * d[i+3];
            b[i+4] = a[i+4] * a_orig[i+5] * d[i+4];
            b[i+5] = a[i+5] * a_orig[i+6] * d[i+5];
            b[i+6] = a[i+6] * a_orig[i+7] * d[i+6];
            b[i+7] = a[i+7] * a_orig[i+8] * d[i+7];
        }
        for (; i < LEN_1D - 1; i++) {
            b[i] = a[i] * a_orig[i+1] * d[i];
        }
    }

    free(a_orig);
}
