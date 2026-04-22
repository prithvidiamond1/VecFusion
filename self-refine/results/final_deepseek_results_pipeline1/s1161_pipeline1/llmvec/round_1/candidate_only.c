#include <stddef.h>

typedef float float32_t;

void vectorized_s1161(int iterations, int LEN_1D, float* a, float* b, float* c, float* d, float* e) {
    for (int nl = 0; nl < iterations; nl++) {
        int i = 0;
        // Vectorizable portion: process in chunks of 4
        for (; i < LEN_1D - 1 - 3; i += 4) {
            float32_t c0 = c[i];
            float32_t c1 = c[i+1];
            float32_t c2 = c[i+2];
            float32_t c3 = c[i+3];
            
            float32_t mask0 = (c0 < 0.0f) ? -1.0f : 0.0f;
            float32_t mask1 = (c1 < 0.0f) ? -1.0f : 0.0f;
            float32_t mask2 = (c2 < 0.0f) ? -1.0f : 0.0f;
            float32_t mask3 = (c3 < 0.0f) ? -1.0f : 0.0f;
            
            // Compute both paths
            float32_t a_val0 = c[i] + d[i] * e[i];
            float32_t b_val0 = a[i] + d[i] * d[i];
            float32_t a_val1 = c[i+1] + d[i+1] * e[i+1];
            float32_t b_val1 = a[i+1] + d[i+1] * d[i+1];
            float32_t a_val2 = c[i+2] + d[i+2] * e[i+2];
            float32_t b_val2 = a[i+2] + d[i+2] * d[i+2];
            float32_t a_val3 = c[i+3] + d[i+3] * e[i+3];
            float32_t b_val3 = a[i+3] + d[i+3] * d[i+3];
            
            // Select based on condition
            a[i]   = (mask0 == 0.0f) ? a_val0 : a[i];
            b[i]   = (mask0 != 0.0f) ? b_val0 : b[i];
            a[i+1] = (mask1 == 0.0f) ? a_val1 : a[i+1];
            b[i+1] = (mask1 != 0.0f) ? b_val1 : b[i+1];
            a[i+2] = (mask2 == 0.0f) ? a_val2 : a[i+2];
            b[i+2] = (mask2 != 0.0f) ? b_val2 : b[i+2];
            a[i+3] = (mask3 == 0.0f) ? a_val3 : a[i+3];
            b[i+3] = (mask3 != 0.0f) ? b_val3 : b[i+3];
        }
        // Scalar cleanup for remaining elements
        for (; i < LEN_1D - 1; ++i) {
            if (c[i] < 0.0f) {
                b[i] = a[i] + d[i] * d[i];
            } else {
                a[i] = c[i] + d[i] * e[i];
            }
        }
    }
}
