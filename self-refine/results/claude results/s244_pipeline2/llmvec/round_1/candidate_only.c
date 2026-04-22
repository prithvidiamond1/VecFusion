#include <stdlib.h>
#include <string.h>

void vectorized_s244(int iterations, int LEN_1D, float* a, float* b, float* c, float* d)
{
    float* orig_a = (float*)malloc(LEN_1D * sizeof(float));

    for (int nl = 0; nl < iterations; nl++) {
        // Store original a values before any updates
        memcpy(orig_a, a, LEN_1D * sizeof(float));

        // Loop 1: compute new b[i] = c[i] + b[i] (original b[i])
        int n1 = LEN_1D - 1;
        int i = 0;
        for (; i <= n1 - 8; i += 8) {
            b[i+0] = c[i+0] + b[i+0];
            b[i+1] = c[i+1] + b[i+1];
            b[i+2] = c[i+2] + b[i+2];
            b[i+3] = c[i+3] + b[i+3];
            b[i+4] = c[i+4] + b[i+4];
            b[i+5] = c[i+5] + b[i+5];
            b[i+6] = c[i+6] + b[i+6];
            b[i+7] = c[i+7] + b[i+7];
        }
        for (; i < n1; i++) {
            b[i] = c[i] + b[i];
        }

        // Loop 2: compute a[i+1] = new_b[i] + orig_a[i+1] * d[i]
        i = 0;
        for (; i <= n1 - 8; i += 8) {
            a[i+1] = b[i+0] + orig_a[i+1] * d[i+0];
            a[i+2] = b[i+1] + orig_a[i+2] * d[i+1];
            a[i+3] = b[i+2] + orig_a[i+3] * d[i+2];
            a[i+4] = b[i+3] + orig_a[i+4] * d[i+3];
            a[i+5] = b[i+4] + orig_a[i+5] * d[i+4];
            a[i+6] = b[i+5] + orig_a[i+6] * d[i+5];
            a[i+7] = b[i+6] + orig_a[i+7] * d[i+6];
            a[i+8] = b[i+7] + orig_a[i+8] * d[i+7];
        }
        for (; i < n1; i++) {
            a[i + 1] = b[i] + orig_a[i + 1] * d[i];
        }

        // Loop 3: compute a[i] = (b[i] - c[i]) + c[i]*d[i]
        i = 0;
        for (; i <= n1 - 8; i += 8) {
            a[i+0] = (b[i+0] - c[i+0]) + c[i+0] * d[i+0];
            a[i+1] = (b[i+1] - c[i+1]) + c[i+1] * d[i+1];
            a[i+2] = (b[i+2] - c[i+2]) + c[i+2] * d[i+2];
            a[i+3] = (b[i+3] - c[i+3]) + c[i+3] * d[i+3];
            a[i+4] = (b[i+4] - c[i+4]) + c[i+4] * d[i+4];
            a[i+5] = (b[i+5] - c[i+5]) + c[i+5] * d[i+5];
            a[i+6] = (b[i+6] - c[i+6]) + c[i+6] * d[i+6];
            a[i+7] = (b[i+7] - c[i+7]) + c[i+7] * d[i+7];
        }
        for (; i < n1; i++) {
            a[i] = (b[i] - c[i]) + c[i] * d[i];
        }
    }

    free(orig_a);
}
