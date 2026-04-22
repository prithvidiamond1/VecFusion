#include <stdint.h>

void vectorized_s342(int iterations, int LEN_1D, float* a, float* b)
{
    for (int nl = 0; nl < iterations; nl++) {
        int masks[LEN_1D];
        int prefix[LEN_1D];

        // First pass: compute masks (unrolled 4x)
        int i = 0;
        for (; i <= LEN_1D - 4; i += 4) {
            masks[i+0] = (a[i+0] > 0.f) ? 1 : 0;
            masks[i+1] = (a[i+1] > 0.f) ? 1 : 0;
            masks[i+2] = (a[i+2] > 0.f) ? 1 : 0;
            masks[i+3] = (a[i+3] > 0.f) ? 1 : 0;
        }
        for (; i < LEN_1D; i++) {
            masks[i] = (a[i] > 0.f) ? 1 : 0;
        }

        // Second pass: prefix sum - sequential dependency, cannot vectorize
        prefix[0] = masks[0] - 1;
        for (i = 1; i < LEN_1D; i++) {
            prefix[i] = prefix[i-1] + masks[i];
        }

        // Third pass: conditional update using precomputed indices (unrolled 4x)
        i = 0;
        for (; i <= LEN_1D - 4; i += 4) {
            int idx0 = prefix[i+0] < 0 ? 0 : prefix[i+0];
            int idx1 = prefix[i+1] < 0 ? 0 : prefix[i+1];
            int idx2 = prefix[i+2] < 0 ? 0 : prefix[i+2];
            int idx3 = prefix[i+3] < 0 ? 0 : prefix[i+3];

            float bval0 = b[idx0];
            float bval1 = b[idx1];
            float bval2 = b[idx2];
            float bval3 = b[idx3];

            int m0 = masks[i+0];
            int m1 = masks[i+1];
            int m2 = masks[i+2];
            int m3 = masks[i+3];

            a[i+0] = m0 * bval0 + (1 - m0) * a[i+0];
            a[i+1] = m1 * bval1 + (1 - m1) * a[i+1];
            a[i+2] = m2 * bval2 + (1 - m2) * a[i+2];
            a[i+3] = m3 * bval3 + (1 - m3) * a[i+3];
        }
        for (; i < LEN_1D; i++) {
            int idx = prefix[i] < 0 ? 0 : prefix[i];
            float bval = b[idx];
            int m = masks[i];
            a[i] = m * bval + (1 - m) * a[i];
        }
    }
}
