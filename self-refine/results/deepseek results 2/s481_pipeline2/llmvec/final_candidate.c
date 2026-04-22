#include <stdint.h>

void vectorized_s481(int iterations, int LEN_1D, float* a, float *b, float *c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        int exit_point = LEN_1D;

        // Find first negative in d[]
        int i;
        for (i = 0; i < LEN_1D; i++) {
            if (d[i] < 0.0f) {
                exit_point = i;
                break;
            }
        }

        // Vectorized computation loop using 4-wide unrolling
        int j = 0;
        int limit4 = exit_point - (exit_point % 4);
        for (j = 0; j < limit4; j += 4) {
            a[j+0] += b[j+0] * c[j+0];
            a[j+1] += b[j+1] * c[j+1];
            a[j+2] += b[j+2] * c[j+2];
            a[j+3] += b[j+3] * c[j+3];
        }
        // Scalar tail
        for (; j < exit_point; j++) {
            a[j] += b[j] * c[j];
        }

        // Early return if negative found
        if (exit_point < LEN_1D) {
            return;
        }
    }
}
