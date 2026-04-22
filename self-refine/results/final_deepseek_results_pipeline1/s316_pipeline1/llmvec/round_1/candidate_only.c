#include <stddef.h>

float vectorized_s316(int iterations, int LEN_1D, float* a) {
    float x;
    int total_outer = iterations * 5;
    for (int nl = 0; nl < total_outer; nl++) {
        x = a[0];
        int i = 1;
        // Vector-like unrolled loop for finding minimum
        for (; i + 4 <= LEN_1D; i += 4) {
            float v0 = a[i];
            float v1 = a[i+1];
            float v2 = a[i+2];
            float v3 = a[i+3];
            if (v0 < x) x = v0;
            if (v1 < x) x = v1;
            if (v2 < x) x = v2;
            if (v3 < x) x = v3;
        }
        // Scalar cleanup for remaining elements
        for (; i < LEN_1D; ++i) {
            if (a[i] < x) {
                x = a[i];
            }
        }
    }
    return x;
}
