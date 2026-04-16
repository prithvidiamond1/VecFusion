#include <math.h>

void vectorized_s451(int iterations, int LEN_1D, float* __restrict__ a, float* __restrict__ b, float* __restrict__ c)
{
    for (int nl = 0; nl < iterations/5; nl++) {
        int i = 0;
        int limit = LEN_1D - (LEN_1D % 4);
        for (; i < limit; i += 4) {
            a[i+0] = sinf(b[i+0]) + cosf(c[i+0]);
            a[i+1] = sinf(b[i+1]) + cosf(c[i+1]);
            a[i+2] = sinf(b[i+2]) + cosf(c[i+2]);
            a[i+3] = sinf(b[i+3]) + cosf(c[i+3]);
        }
        for (; i < LEN_1D; i++) {
            a[i] = sinf(b[i]) + cosf(c[i]);
        }
    }
}
