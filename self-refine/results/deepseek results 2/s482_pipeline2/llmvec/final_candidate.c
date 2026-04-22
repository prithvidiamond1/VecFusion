#include <stddef.h>

void vectorized_s482(int iterations, int LEN_1D, float* a, float *b, float *c)
{
    for (int nl = 0; nl < iterations; nl++) {
        int break_idx = LEN_1D;

        // Find the first index where c[i] > b[i], while updating a[i] up to that point
        for (int i = 0; i < LEN_1D; i++) {
            a[i] += b[i] * c[i];
            if (c[i] > b[i]) {
                break_idx = i;
                break;
            }
        }

        // Second loop is a no-op, nothing to do
        (void)break_idx;
    }
}
