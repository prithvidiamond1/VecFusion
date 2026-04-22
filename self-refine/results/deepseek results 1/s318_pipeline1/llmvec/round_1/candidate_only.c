#include <math.h>

float vectorized_s318(int iterations, int LEN_1D, float* a, int inc)
{
    int k, index;
    float max, chksum;
    for (int nl = 0; nl < iterations/2; nl++) {
        k = 0;
        index = 0;
        max = fabsf(a[0]);
        k += inc;

        // Process in chunks of 4 (unrolled for vectorization friendliness)
        // But since we have a data-dependent index tracking (find max abs),
        // we can vectorize by tracking max and index in parallel lanes.

        // Use a simple unrolled approach: process 4 elements at a time
        // keeping track of max and index across lanes.
        float max0 = max, max1 = -1.0f, max2 = -1.0f, max3 = -1.0f;
        int idx0 = 0, idx1 = 0, idx2 = 0, idx3 = 0;
        int i = 1;

        // We need to handle the strided access a[k] where k = i*inc
        // So a[k] = a[i * inc] for i = 1, 2, ...
        // Let's just unroll by 4 with scalar ops but in a vectorization-friendly pattern

        int limit = 1 + ((LEN_1D - 1) / 4) * 4;
        // Actually let's do it simply: unroll by 4
        int end4 = 1 + ((LEN_1D - 1) & ~3);

        for (; i < end4; i += 4) {
            float v0 = fabsf(a[k]);
            float v1 = fabsf(a[k + inc]);
            float v2 = fabsf(a[k + 2*inc]);
            float v3 = fabsf(a[k + 3*inc]);

            if (v0 > max0) { max0 = v0; idx0 = i; }
            if (v1 > max0) { max0 = v1; idx0 = i+1; }
            if (v2 > max0) { max0 = v2; idx0 = i+2; }
            if (v3 > max0) { max0 = v3; idx0 = i+3; }

            k += 4*inc;
        }

        // Scalar cleanup
        for (; i < LEN_1D; i++) {
            float v = fabsf(a[k]);
            if (v > max0) {
                max0 = v;
                idx0 = i;
            }
            k += inc;
        }

        max = max0;
        index = idx0;

        chksum = max + (float)index;
    }
    return max + index + 1;
}
