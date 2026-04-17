#include <stdint.h>

void vectorized_s291(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < 2*iterations; nl++) {
        a[0] = (b[0] + b[LEN_1D-1]) * 0.5f;

        int i = 1;
        // Vectorize using groups of 8
        int limit = LEN_1D - 7;
        for (; i < limit; i += 8) {
            a[i+0] = (b[i+0] + b[i-1]) * 0.5f;
            a[i+1] = (b[i+1] + b[i+0]) * 0.5f;
            a[i+2] = (b[i+2] + b[i+1]) * 0.5f;
            a[i+3] = (b[i+3] + b[i+2]) * 0.5f;
            a[i+4] = (b[i+4] + b[i+3]) * 0.5f;
            a[i+5] = (b[i+5] + b[i+4]) * 0.5f;
            a[i+6] = (b[i+6] + b[i+5]) * 0.5f;
            a[i+7] = (b[i+7] + b[i+6]) * 0.5f;
        }
        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            a[i] = (b[i] + b[i-1]) * 0.5f;
        }
    }
}
