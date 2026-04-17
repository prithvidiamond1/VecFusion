#include <stdint.h>

void vectorized_s221(int iterations, int LEN_1D, float*a, float*b, float*c, float*d)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        // First loop: a[i] += c[i] * d[i] for i in [1, LEN_1D)
        // This loop has no dependencies, can be vectorized
        int i = 1;
        int len = LEN_1D - 1; // number of elements to process

        // Unroll by 8 for SIMD-friendly code
        int limit = 1 + (len / 8) * 8;
        for (; i < limit; i += 8) {
            a[i+0] += c[i+0] * d[i+0];
            a[i+1] += c[i+1] * d[i+1];
            a[i+2] += c[i+2] * d[i+2];
            a[i+3] += c[i+3] * d[i+3];
            a[i+4] += c[i+4] * d[i+4];
            a[i+5] += c[i+5] * d[i+5];
            a[i+6] += c[i+6] * d[i+6];
            a[i+7] += c[i+7] * d[i+7];
        }
        // Scalar cleanup
        for (; i < LEN_1D; i++) {
            a[i] += c[i] * d[i];
        }

        // Second loop: b[i] = b[i-1] + a[i] + d[i] for i in [1, LEN_1D)
        // This loop has a loop-carried dependency on b[i-1], cannot be vectorized
        for (i = 1; i < LEN_1D; i++) {
            b[i] = b[i - 1] + a[i] + d[i];
        }
    }
}
