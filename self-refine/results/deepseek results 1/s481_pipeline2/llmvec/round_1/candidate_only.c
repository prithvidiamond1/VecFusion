#include <stdint.h>

void vectorized_s481(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    for (int nl = 0; nl < iterations; nl++) {
        // First loop: check for negative values
        int should_return = 0;
        int i = 0;

        // Vectorized check using 4-wide unrolling
        for (; i <= LEN_1D - 4; i += 4) {
            if (d[i] < 0.0f || d[i+1] < 0.0f || d[i+2] < 0.0f || d[i+3] < 0.0f) {
                should_return = 1;
                break;
            }
        }
        // Scalar tail for first loop
        if (!should_return) {
            for (; i < LEN_1D; i++) {
                if (d[i] < 0.0f) {
                    should_return = 1;
                    break;
                }
            }
        }

        if (should_return) return;

        // Second loop: vectorized computation
        i = 0;
        for (; i <= LEN_1D - 8; i += 8) {
            a[i+0] += b[i+0] * c[i+0];
            a[i+1] += b[i+1] * c[i+1];
            a[i+2] += b[i+2] * c[i+2];
            a[i+3] += b[i+3] * c[i+3];
            a[i+4] += b[i+4] * c[i+4];
            a[i+5] += b[i+5] * c[i+5];
            a[i+6] += b[i+6] * c[i+6];
            a[i+7] += b[i+7] * c[i+7];
        }
        // Scalar tail for second loop
        for (; i < LEN_1D; i++) {
            a[i] += b[i] * c[i];
        }
    }
}
