#include <stddef.h>

void vectorized_s482(int iterations, int LEN_1D, float* a, float *b, float *c) {
    typedef float v4f __attribute__((vector_size(16)));
    const size_t VF = 4;
    
    for (int nl = 0; nl < iterations; nl++) {
        // First loop: find break point (remains scalar)
        int break_index = LEN_1D;
        for (int i = 0; i < LEN_1D; i++) {
            if (c[i] > b[i]) {
                break_index = i;
                break;
            }
        }
        
        // Second loop: vectorized computation up to break point
        int i = 0;
        if (break_index >= VF) {
            for (; i <= break_index - VF; i += VF) {
                v4f b_vec = *(v4f*)&b[i];
                v4f c_vec = *(v4f*)&c[i];
                v4f a_vec = *(v4f*)&a[i];
                a_vec += b_vec * c_vec;
                *(v4f*)&a[i] = a_vec;
            }
        }
        
        // Scalar cleanup for remaining iterations
        for (; i < break_index; i++) {
            a[i] += b[i] * c[i];
        }
        
        // Handle the break point iteration if it exists
        if (break_index < LEN_1D) {
            a[break_index] += b[break_index] * c[break_index];
        }
    }
}
