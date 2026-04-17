#include <stdint.h>

void vectorized_s453(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < iterations * 2; nl++) {
        int i = 0;
        // Unroll by 4 for vector-friendly code
        for (; i <= LEN_1D - 4; i += 4) {
            float coeff0 = (float)(2 * (i + 1));
            float coeff1 = (float)(2 * (i + 2));
            float coeff2 = (float)(2 * (i + 3));
            float coeff3 = (float)(2 * (i + 4));
            a[i + 0] = coeff0 * b[i + 0];
            a[i + 1] = coeff1 * b[i + 1];
            a[i + 2] = coeff2 * b[i + 2];
            a[i + 3] = coeff3 * b[i + 3];
        }
        // Scalar tail
        for (; i < LEN_1D; i++) {
            a[i] = (float)(2 * (i + 1)) * b[i];
        }
    }
}
