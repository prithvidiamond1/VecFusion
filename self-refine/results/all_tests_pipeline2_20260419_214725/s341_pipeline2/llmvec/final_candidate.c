#include <stddef.h>

void vectorized_s341(int iterations, int LEN_1D, float* a, float* b) {
    for (int nl = 0; nl < iterations; nl++) {
        int write_idx = 0;
        
        // Vectorized condition check and write
        typedef float v4f __attribute__((vector_size(16)));
        typedef int v4i __attribute__((vector_size(16)));
        const int VEC_SIZE = 4;
        int i = 0;
        
        for (; i + VEC_SIZE <= LEN_1D; i += VEC_SIZE) {
            // Load 4 floats from b
            v4f b_vec = *(v4f*)&b[i];
            // Compare with 0.0f
            v4i mask_vec = b_vec > (v4f){0.0f, 0.0f, 0.0f, 0.0f};
            
            // Extract mask to scalar
            int mask[VEC_SIZE];
            *(v4i*)mask = mask_vec;
            
            // Process each element in the vector
            for (int j = 0; j < VEC_SIZE; j++) {
                if (mask[j]) {
                    a[write_idx] = b[i + j];
                    write_idx++;
                }
            }
        }
        
        // Scalar tail cleanup
        for (; i < LEN_1D; i++) {
            if (b[i] > 0.0f) {
                a[write_idx] = b[i];
                write_idx++;
            }
        }
    }
}
