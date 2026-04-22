#include <stddef.h>

void vectorized_s116(int iterations, int LEN_1D, float* a) {
    int total_iterations = iterations * 10;
    for (int nl = 0; nl < total_iterations; nl++) {
        int i = 0;
        // Vectorizable portion: process groups of 5 elements
        for (; i < LEN_1D - 5; i += 5) {
            float t0 = a[i + 1] * a[i];
            float t1 = a[i + 2] * a[i + 1];
            float t2 = a[i + 3] * a[i + 2];
            float t3 = a[i + 4] * a[i + 3];
            float t4 = a[i + 5] * a[i + 4];
            a[i] = t0;
            a[i + 1] = t1;
            a[i + 2] = t2;
            a[i + 3] = t3;
            a[i + 4] = t4;
        }
        // Scalar cleanup for remaining elements (if any)
        for (; i < LEN_1D - 5; i++) {
            a[i] = a[i + 1] * a[i];
        }
    }
}
