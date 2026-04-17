#include <stddef.h>

float vectorized_s314(int iterations, int LEN_1D, float* a)
{
    float x;
    for (int nl = 0; nl < iterations * 5; nl++) {
        // Use 4-wide unrolling to find maximum
        float x0, x1, x2, x3;
        x0 = a[0];
        x1 = a[0];
        x2 = a[0];
        x3 = a[0];

        int i = 0;
        int limit = LEN_1D - (LEN_1D % 4);
        for (; i < limit; i += 4) {
            float v0 = a[i + 0];
            float v1 = a[i + 1];
            float v2 = a[i + 2];
            float v3 = a[i + 3];
            if (v0 > x0) x0 = v0;
            if (v1 > x1) x1 = v1;
            if (v2 > x2) x2 = v2;
            if (v3 > x3) x3 = v3;
        }
        // Scalar tail
        for (; i < LEN_1D; i++) {
            if (a[i] > x0) x0 = a[i];
        }
        // Reduce across lanes
        if (x1 > x0) x0 = x1;
        if (x2 > x0) x0 = x2;
        if (x3 > x0) x0 = x3;
        x = x0;
    }
    return x;
}
