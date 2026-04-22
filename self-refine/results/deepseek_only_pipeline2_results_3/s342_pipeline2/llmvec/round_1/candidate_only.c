#include <stddef.h>

void vectorized_s342(int iterations, int LEN_1D, float* a, float *b) {
    for (int nl = 0; nl < iterations; nl++) {
        // First pass: count positive elements
        int count = 0;
        int i = 0;
        
        // Vectorized counting loop
        typedef float v4f __attribute__((vector_size(16)));
        const int VEC_SIZE = 4;
        int vec_loop_bound = LEN_1D - (LEN_1D % VEC_SIZE);
        
        for (; i < vec_loop_bound; i += VEC_SIZE) {
            v4f va = *(v4f*)&a[i];
            v4f zero_vec = (v4f){0.0f, 0.0f, 0.0f, 0.0f};
            v4f cmp_result = va > zero_vec;
            
            // Extract comparison results
            int cmp_mask = 0;
            for (int k = 0; k < VEC_SIZE; k++) {
                if (cmp_result[k] != 0) {
                    cmp_mask |= (1 << k);
                }
            }
            
            // Count set bits in mask
            count += __builtin_popcount(cmp_mask);
        }
        
        // Scalar tail for counting
        for (; i < LEN_1D; i++) {
            if (a[i] > 0.0f) {
                count++;
            }
        }
        
        // Second pass: update positive elements
        int j = -1;
        i = 0;
        vec_loop_bound = LEN_1D - (LEN_1D % VEC_SIZE);
        
        // Vectorized update loop
        for (; i < vec_loop_bound; i += VEC_SIZE) {
            v4f va = *(v4f*)&a[i];
            v4f zero_vec = (v4f){0.0f, 0.0f, 0.0f, 0.0f};
            v4f cmp_result = va > zero_vec;
            
            // Process each element in the vector
            for (int k = 0; k < VEC_SIZE; k++) {
                if (cmp_result[k] != 0) {
                    j++;
                    a[i + k] = b[j];
                }
            }
        }
        
        // Scalar tail for update
        for (; i < LEN_1D; i++) {
            if (a[i] > 0.0f) {
                j++;
                a[i] = b[j];
            }
        }
    }
}
