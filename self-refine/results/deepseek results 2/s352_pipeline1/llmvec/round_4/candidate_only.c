#include <stdint.h>

float vectorized_s352(int iterations, int LEN_1D, float* a, float* b)
{
    float dot;
    for (int nl = 0; nl < 8 * iterations; nl++) {
        dot = (float)0.;
        for (int i = 0; i < LEN_1D; i++) {
            dot = dot + a[i] * b[i];
        }
    }
    return dot;
}
