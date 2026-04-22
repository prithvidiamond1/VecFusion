#include <stddef.h>

typedef float float32x4_t __attribute__((__vector_size__(16)));

void vectorized_s1213(int iterations, int LEN_1D, float* a, float* b, float* c, float* d) {
    if (LEN_1D < 3) return;

    for (int nl = 0; nl < iterations; nl++) {
        int i = 1;
        int last = LEN_1D - 1;
        
        // Vectorize main loop: process 4 elements at a time
        for (; i <= last - 4; i += 4) {
            // First pass: compute a values using original b values
            float32x4_t b_orig = *(float32x4_t*)(b + i - 1);
            float32x4_t c_vec = *(float32x4_t*)(c + i);
            float32x4_t a_new = b_orig + c_vec;
            *(float32x4_t*)(a + i) = a_new;
            
            // Second pass: compute b values using updated a values
            float32x4_t a_next = *(float32x4_t*)(a + i + 1);
            float32x4_t d_vec = *(float32x4_t*)(d + i);
            float32x4_t b_new = a_next * d_vec;
            *(float32x4_t*)(b + i) = b_new;
        }
        
        // Scalar cleanup for remaining elements
        for (int k = i; k < last; k++) {
            a[k] = b[k-1] + c[k];
            b[k] = a[k+1] * d[k];
        }
    }
}
