#include <stddef.h>

void vectorized_s141(int iterations, float * flat_2d_array, float bb[256][256])
{
    int nl_iterations = 200 * (iterations / 256);
    
    for (int nl = 0; nl < nl_iterations; nl++) {
        for (int j = 0; j < 256; j++) {
            int base_idx = j * (j + 1) / 2;
            
            // Vectorize with explicit vector width
            typedef float v4f __attribute__((vector_size(16)));
            const int VEC_WIDTH = 4;
            
            // Compute vectorized iteration count
            int vec_limit = (j + 1) - ((j + 1) % VEC_WIDTH);
            
            // Vectorized loop
            int i;
            for (i = 0; i < vec_limit; i += VEC_WIDTH) {
                v4f flat_vec, bb_vec;
                
                // Load from flat_2d_array
                flat_vec = *(v4f *)&flat_2d_array[base_idx + i];
                
                // Load from bb[j][i..i+3]
                bb_vec = *(v4f *)&bb[j][i];
                
                // Add
                flat_vec += bb_vec;
                
                // Store back
                *(v4f *)&flat_2d_array[base_idx + i] = flat_vec;
            }
            
            // Scalar cleanup for remaining elements
            for (; i <= j; i++) {
                flat_2d_array[base_idx + i] += bb[j][i];
            }
        }
    }
}
