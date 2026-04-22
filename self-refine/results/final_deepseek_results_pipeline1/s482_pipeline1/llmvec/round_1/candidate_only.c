#include <stddef.h>

typedef float float32_t __attribute__((ext_vector_type(4)));

void vectorized_s482(int iterations, int LEN_1D, float* a, float *b, float *c) {
    for (int nl = 0; nl < iterations; nl++) {
        int i = 0;
        // Vectorized main loop
        for (; i <= LEN_1D - 4; i += 4) {
            float32_t va = *(float32_t*)(a + i);
            float32_t vb = *(float32_t*)(b + i);
            float32_t vc = *(float32_t*)(c + i);
            va += vb * vc;
            *(float32_t*)(a + i) = va;
            
            // Check break condition for each element
            float32_t cmp = vc > vb;
            int mask = __builtin_convertvector(cmp, int32_t __attribute__((ext_vector_type(4))));
            if (mask) {
                // Find first true element
                int first = __builtin_ctz(mask);
                i += first;
                goto end_loop;
            }
        }
        // Scalar cleanup
        for (; i < LEN_1D; i++) {
            a[i] += b[i] * c[i];
            if (c[i] > b[i]) break;
        }
        end_loop:;
    }
}
