#include <stddef.h>

typedef float float32x4_t __attribute__((__vector_size__(16)));

void vectorized_s1213(int iterations, int LEN_1D, float* a, float* b, float* c, float* d) {
    if (LEN_1D < 3) return;

    for (int nl = 0; nl < iterations; nl++) {
        // Process first element separately to handle the dependency
        int i = 1;
        if (i < LEN_1D - 1) {
            a[i] = b[i-1] + c[i];
            b[i] = a[i+1] * d[i];
            i++;
        }

        // Vectorize remaining elements: process a and b in separate passes
        // First pass: compute all a values using original b values
        for (; i <= LEN_1D - 5; i += 4) {
            float32x4_t b_orig = *(float32x4_t*)(b + i - 1);
            float32x4_t c_vec = *(float32x4_t*)(c + i);
            float32x4_t a_new = b_orig + c_vec;
            *(float32x4_t*)(a + i) = a_new;
        }

        // Second pass: compute all b values using updated a values
        for (int j = 1; j < i; j += 4) {
            float32x4_t a_next = *(float32x4_t*)(a + j + 1);
            float32x4_t d_vec = *(float32x4_t*)(d + j);
            float32x4_t b_new = a_next * d_vec;
            *(float32x4_t*)(b + j) = b_new;
        }

        // Scalar cleanup for remaining elements
        for (; i < LEN_1D - 1; i++) {
            a[i] = b[i-1] + c[i];
            b[i] = a[i+1] * d[i];
        }
    }
}
