#include <stdint.h>

void s1113(int iterations, int LEN_1D, float* a, float* b)
{
    int half = LEN_1D / 2;
    for (int nl = 0; nl < 2*iterations; nl++) {
        // Before index half is reached, a[half] is the old value.
        // Once i == half, a[half] gets updated, and subsequent iterations use the new value.
        // We split into two phases: i < half and i >= half.

        // Phase 1: i < half, a[half] hasn't been written yet, safe to hoist
        float scalar_val = a[half];
        int i = 0;
        for (; i <= half - 8; i += 8) {
            a[i+0] = scalar_val + b[i+0];
            a[i+1] = scalar_val + b[i+1];
            a[i+2] = scalar_val + b[i+2];
            a[i+3] = scalar_val + b[i+3];
            a[i+4] = scalar_val + b[i+4];
            a[i+5] = scalar_val + b[i+5];
            a[i+6] = scalar_val + b[i+6];
            a[i+7] = scalar_val + b[i+7];
        }
        for (; i < half; i++) {
            a[i] = scalar_val + b[i];
        }

        // Phase 2: i >= half, a[half] may have been updated, read live each iteration
        for (; i < LEN_1D; i++) {
            a[i] = a[half] + b[i];
        }
    }
}