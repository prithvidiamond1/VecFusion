#include <stdint.h>

float vectorized_s352(int iterations, int LEN_1D, float* a, float *b)
{
    float dot = 0.0f;
    for (int nl = 0; nl < 8 * iterations; nl++) {
        dot = 0.0f;

        float dot0 = 0.0f, dot1 = 0.0f, dot2 = 0.0f, dot3 = 0.0f, dot4 = 0.0f;
        int i;

        for (i = 0; i <= LEN_1D - 5; i += 5) {
            dot0 += a[i+0] * b[i+0];
            dot1 += a[i+1] * b[i+1];
            dot2 += a[i+2] * b[i+2];
            dot3 += a[i+3] * b[i+3];
            dot4 += a[i+4] * b[i+4];
        }

        for (; i < LEN_1D; i++) {
            dot += a[i] * b[i];
        }

        dot += dot0 + dot1 + dot2 + dot3 + dot4;
    }
    return dot;
}
