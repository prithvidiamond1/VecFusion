#include <stdint.h>

void vectorized_s161(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e)
{
    for (int nl = 0; nl < iterations/2; nl++) {
        int i = 0;
        // Process 4 elements at a time with scalar logic (unrolled)
        // Note: c[i+1] write dependency prevents true SIMD, but unrolling
        // helps compiler understand the pattern
        for (; i < LEN_1D - 1 - 3; i += 4) {
            // Element i+0
            if (b[i] < 0.0f) {
                c[i+1] = a[i] + d[i] * d[i];
            } else {
                a[i] = c[i] + d[i] * e[i];
            }
            // Element i+1
            if (b[i+1] < 0.0f) {
                c[i+2] = a[i+1] + d[i+1] * d[i+1];
            } else {
                a[i+1] = c[i+1] + d[i+1] * e[i+1];
            }
            // Element i+2
            if (b[i+2] < 0.0f) {
                c[i+3] = a[i+2] + d[i+2] * d[i+2];
            } else {
                a[i+2] = c[i+2] + d[i+2] * e[i+2];
            }
            // Element i+3
            if (b[i+3] < 0.0f) {
                c[i+4] = a[i+3] + d[i+3] * d[i+3];
            } else {
                a[i+3] = c[i+3] + d[i+3] * e[i+3];
            }
        }
        // Scalar cleanup tail
        for (; i < LEN_1D - 1; i++) {
            if (b[i] < 0.0f) {
                c[i+1] = a[i] + d[i] * d[i];
            } else {
                a[i] = c[i] + d[i] * e[i];
            }
        }
    }
}
