#include <stdint.h>

float vectorized_s352(int iterations, int LEN_1D, float* a, float *b) {
    float dot = 0.0f;
    for (int nl = 0; nl < 8 * iterations; nl++) {
        float dot0 = 0.0f, dot1 = 0.0f, dot2 = 0.0f, dot3 = 0.0f;
        int i = 0;
        int limit4 = LEN_1D & ~3;

        // Process groups of 4 using vector-friendly unrolling
        for (i = 0; i < limit4; i += 4) {
            dot0 += a[i]     * b[i];
            dot1 += a[i + 1] * b[i + 1];
            dot2 += a[i + 2] * b[i + 2];
            dot3 += a[i + 3] * b[i + 3];
        }

        // Scalar cleanup for remaining elements
        for (; i < LEN_1D; i++) {
            dot0 += a[i] * b[i];
        }

        dot = dot0 + dot1 + dot2 + dot3;
    }
    return dot;
}
