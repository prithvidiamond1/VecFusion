#include <string.h>

void vectorized_s293(int iterations, int LEN_1D, float* a)
{
    for (int nl = 0; nl < 4 * iterations; nl++) {
        float val = a[0];
        int i = 0;
        // Unroll by 8 for vector-friendly code
        for (; i <= LEN_1D - 8; i += 8) {
            a[i + 0] = val;
            a[i + 1] = val;
            a[i + 2] = val;
            a[i + 3] = val;
            a[i + 4] = val;
            a[i + 5] = val;
            a[i + 6] = val;
            a[i + 7] = val;
        }
        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            a[i] = val;
        }
    }
}
