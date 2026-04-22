#include <stdint.h>

void vectorized_s1113(int iterations, int LEN_1D, float* a, float* b)
{
    float scalar_val = a[LEN_1D/2];  // capture once before any writes
    for (int nl = 0; nl < 2*iterations; nl++) {
        int i = 0;
        int limit = LEN_1D - (LEN_1D % 8);
        for (; i < limit; i += 8) {
            a[i+0] = scalar_val + b[i+0];
            a[i+1] = scalar_val + b[i+1];
            a[i+2] = scalar_val + b[i+2];
            a[i+3] = scalar_val + b[i+3];
            a[i+4] = scalar_val + b[i+4];
            a[i+5] = scalar_val + b[i+5];
            a[i+6] = scalar_val + b[i+6];
            a[i+7] = scalar_val + b[i+7];
        }
        for (; i < LEN_1D; i++) {
            a[i] = scalar_val + b[i];
        }
    }
}
