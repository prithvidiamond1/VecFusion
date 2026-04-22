#include <stddef.h>
#include <stdint.h>

typedef float float32_t __attribute__((ext_vector_type(4)));

void vectorized_s341(int iterations, int LEN_1D, float* a, float *b)
{
    for (int nl = 0; nl < iterations; nl++) {
        int j = -1;
        int i = 0;
        
        // Vectorized main loop
        for (; i + 4 <= LEN_1D; i += 4) {
            float32_t vb = *(float32_t*)(b + i);
            float32_t zero = (float32_t){0.0f, 0.0f, 0.0f, 0.0f};
            float32_t mask = vb > zero;
            
            // Extract mask bits
            int mask_bits = __builtin_convertvector(mask, int32_t);
            
            // Process each element
            for (int k = 0; k < 4; k++) {
                if (mask_bits & (1 << k)) {
                    j++;
                    a[j] = b[i + k];
                }
            }
        }
        
        // Scalar cleanup tail
        for (; i < LEN_1D; i++) {
            if (b[i] > 0.0f) {
                j++;
                a[j] = b[i];
            }
        }
    }
}
