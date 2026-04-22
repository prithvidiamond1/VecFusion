#include <math.h>
#include <stdint.h>

float vectorized_s318(int iterations, int LEN_1D, float* a, int inc)
{
    int index;
    float max, chksum;
    chksum = 0.0f;
    max = 0.0f;
    index = 0;

    for (int nl = 0; nl < iterations/2; nl++) {
        index = 0;
        max = fabsf(a[0]);

        /* First pass: find max value */
        int i = 1;
        for (; i <= LEN_1D - 4; i += 4) {
            float v0 = fabsf(a[i]);
            float v1 = fabsf(a[i+1]);
            float v2 = fabsf(a[i+2]);
            float v3 = fabsf(a[i+3]);
            if (v0 > max) max = v0;
            if (v1 > max) max = v1;
            if (v2 > max) max = v2;
            if (v3 > max) max = v3;
        }
        for (; i < LEN_1D; i++) {
            float val = fabsf(a[i]);
            if (val > max) max = val;
        }

        /* Second pass: find first index where value equals max */
        int found = 0;
        /* Check index 0 first */
        if (fabsf(a[0]) == max) {
            index = 0;
            found = 1;
        }

        i = 1;
        for (; i <= LEN_1D - 4 && !found; i += 4) {
            float v0 = fabsf(a[i]);
            float v1 = fabsf(a[i+1]);
            float v2 = fabsf(a[i+2]);
            float v3 = fabsf(a[i+3]);
            int m0 = (v0 == max);
            int m1 = (v1 == max);
            int m2 = (v2 == max);
            int m3 = (v3 == max);
            if (m0 & !found) { index = i;   found = 1; }
            if (m1 & !found) { index = i+1; found = 1; }
            if (m2 & !found) { index = i+2; found = 1; }
            if (m3 & !found) { index = i+3; found = 1; }
        }
        for (; i < LEN_1D && !found; i++) {
            float val = fabsf(a[i]);
            if (val == max) { index = i; found = 1; }
        }

        chksum = max + (float)index;
    }
    return max + index + 1;
}
