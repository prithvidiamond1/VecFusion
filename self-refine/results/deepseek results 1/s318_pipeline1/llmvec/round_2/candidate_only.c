#include <math.h>

float vectorized_s318(int iterations, int LEN_1D, float* a, int inc)
{
    int k, index;
    float max, chksum;
    chksum = 0.0f;
    max = 0.0f;
    index = 0;
    for (int nl = 0; nl < iterations/2; nl++) {
        k = 0;
        index = 0;
        max = fabsf(a[0]);
        k += inc;

        float cur_max = max;
        int cur_idx = 0;

        int i = 1;
        /* Unrolled by 4, stop when fewer than 4 elements remain */
        int end4 = LEN_1D - 3;

        for (; i < end4; i += 4) {
            float v0 = fabsf(a[k]);
            float v1 = fabsf(a[k + inc]);
            float v2 = fabsf(a[k + 2*inc]);
            float v3 = fabsf(a[k + 3*inc]);

            if (v0 > cur_max) { cur_max = v0; cur_idx = i; }
            if (v1 > cur_max) { cur_max = v1; cur_idx = i+1; }
            if (v2 > cur_max) { cur_max = v2; cur_idx = i+2; }
            if (v3 > cur_max) { cur_max = v3; cur_idx = i+3; }

            k += 4*inc;
        }

        /* Scalar cleanup for remaining elements */
        for (; i < LEN_1D; i++) {
            float v = fabsf(a[k]);
            if (v > cur_max) {
                cur_max = v;
                cur_idx = i;
            }
            k += inc;
        }

        max = cur_max;
        index = cur_idx;

        chksum = max + (float)index;
    }
    return chksum;
}
