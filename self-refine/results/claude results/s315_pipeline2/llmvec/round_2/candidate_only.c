#include <float.h>

float vectorized_s315(int iterations, int LEN_1D, float* a)
{
    for (int i = 0; i < LEN_1D; i++)
        a[i] = (i * 7) % LEN_1D;

    float x, chksum;
    int index;

    for (int nl = 0; nl < iterations; nl++) {
        /* First pass: find the maximum value using unrolled reduction */
        float max0 = -FLT_MAX, max1 = -FLT_MAX, max2 = -FLT_MAX, max3 = -FLT_MAX;

        int i = 0;
        int limit = LEN_1D - (LEN_1D % 4);
        for (; i < limit; i += 4) {
            float v0 = a[i+0];
            float v1 = a[i+1];
            float v2 = a[i+2];
            float v3 = a[i+3];
            max0 = v0 > max0 ? v0 : max0;
            max1 = v1 > max1 ? v1 : max1;
            max2 = v2 > max2 ? v2 : max2;
            max3 = v3 > max3 ? v3 : max3;
        }
        for (; i < LEN_1D; i++) {
            float v = a[i];
            max0 = v > max0 ? v : max0;
        }
        float tmp0 = max0 > max1 ? max0 : max1;
        float tmp1 = max2 > max3 ? max2 : max3;
        float max_val = tmp0 > tmp1 ? tmp0 : tmp1;

        /* Second pass: find minimum index where a[i] == max_val */
        index = 0;
        for (int j = 0; j < LEN_1D; j++) {
            if (a[j] == max_val) { index = j; break; }
        }

        x = max_val;
        chksum = x + (float)index;
        (void)chksum;
    }
    return index + x + 1;
}
