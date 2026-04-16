#include <stdint.h>

void vectorized_s1113(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < 2*iterations; nl++) {
        float mid = a[LEN_1D/2];
        for (int i = 0; i < LEN_1D; i++) {
            a[i] = mid + b[i];
        }
    }
}
