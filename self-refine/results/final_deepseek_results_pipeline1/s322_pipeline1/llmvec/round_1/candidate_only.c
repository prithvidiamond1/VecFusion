#include <stddef.h>
#include <stdint.h>

typedef float float32_t __attribute__((ext_vector_type(4)));

void vectorized_s322(int iterations, int LEN_1D, float* a, float *b, float *c) {
    int total_iters = iterations / 2;
    if (total_iters <= 0 || LEN_1D <= 2) return;
    
    for (int nl = 0; nl < total_iters; nl++) {
        int i = 2;
        // Vectorize with 4-wide SIMD
        for (; i + 3 < LEN_1D; i += 4) {
            float32_t a_vec = *(float32_t*)(a + i);
            float32_t a_prev1 = *(float32_t*)(a + i - 1);
            float32_t a_prev2 = *(float32_t*)(a + i - 2);
            float32_t b_vec = *(float32_t*)(b + i);
            float32_t c_vec = *(float32_t*)(c + i);
            
            float32_t result = a_vec + a_prev1 * b_vec + a_prev2 * c_vec;
            *(float32_t*)(a + i) = result;
        }
        // Scalar cleanup for remaining elements
        for (; i < LEN_1D; i++) {
            a[i] = a[i] + a[i - 1] * b[i] + a[i - 2] * c[i];
        }
    }
}
