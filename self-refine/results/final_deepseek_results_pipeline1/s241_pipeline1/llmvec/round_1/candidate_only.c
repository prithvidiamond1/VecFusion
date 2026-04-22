#include <stddef.h>

typedef float float32_t __attribute__((ext_vector_type(4)));

void vectorized_s241(int iterations, int LEN_1D, float* a, float* b, float* c, float* d) {
    int total_iterations = 2 * iterations;
    int n = LEN_1D - 1;
    
    for (int nl = 0; nl < total_iterations; nl++) {
        int i = 0;
        // Vectorized main loop (process 4 elements at a time)
        for (; i <= n - 4; i += 4) {
            float32_t b_vec = *(float32_t*)(b + i);
            float32_t c_vec = *(float32_t*)(c + i);
            float32_t d_vec = *(float32_t*)(d + i);
            float32_t a_next = *(float32_t*)(a + i + 1);
            
            // a[i] = b[i] * c[i] * d[i]
            float32_t a_vec = b_vec * c_vec * d_vec;
            *(float32_t*)(a + i) = a_vec;
            
            // b[i] = a[i] * a[i+1] * d[i]
            // Need to shift a_next for the multiplication
            float32_t a_shifted;
            a_shifted[0] = a_vec[0] * a_next[0];
            a_shifted[1] = a_vec[1] * a_next[1];
            a_shifted[2] = a_vec[2] * a_next[2];
            a_shifted[3] = a_vec[3] * a_next[3];
            
            *(float32_t*)(b + i) = a_shifted * d_vec;
        }
        
        // Scalar cleanup for remaining elements
        for (; i < n; i++) {
            a[i] = b[i] * c[i] * d[i];
            b[i] = a[i] * a[i+1] * d[i];
        }
    }
}
