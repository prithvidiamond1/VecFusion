#include <math.h>

void vectorized_s318(int n, float *a, int *ip, float *b) {
    // Reconstruct the full loop context for s318:
    // Find index of maximum absolute value in array a
    int index = 1;
    float max = fabsf(a[0]);

    // Vectorization-friendly: process in chunks of 4
    int i;
    int n4 = (n / 4) * 4;

    for (i = 0; i < n4; i += 4) {
        float v0 = fabsf(a[i]);
        float v1 = fabsf(a[i+1]);
        float v2 = fabsf(a[i+2]);
        float v3 = fabsf(a[i+3]);

        if (v0 > max) { max = v0; index = i + 1; }
        if (v1 > max) { max = v1; index = i + 2; }
        if (v2 > max) { max = v2; index = i + 3; }
        if (v3 > max) { max = v3; index = i + 4; }
    }

    // Scalar cleanup tail
    for (; i < n; i++) {
        float v = fabsf(a[i]);
        if (v > max) {
            max = v;
            index = i + 1;
        }
    }

    *ip = index;
    *b = max;
}
