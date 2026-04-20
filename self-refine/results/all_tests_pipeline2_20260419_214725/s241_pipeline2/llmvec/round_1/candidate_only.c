#include <stddef.h>

void vectorized_s241(int iterations, int LEN_1D, float* a, float* b, float* c, float* d) {
    int outer_iterations = 2 * iterations;
    int inner_len = LEN_1D - 1;
    
    for (int nl = 0; nl < outer_iterations; nl++) {
        // Vectorized main loop
        int i = 0;
        for (; i + 3 < inner_len; i += 4) {
            // Load original a[i+1] values
            float a_next0 = a[i + 1];
            float a_next1 = a[i + 2];
            float a_next2 = a[i + 3];
            float a_next3 = a[i + 4];
            
            // Compute a[i] = b[i] * c[i] * d[i]
            float a_val0 = b[i] * c[i] * d[i];
            float a_val1 = b[i + 1] * c[i + 1] * d[i + 1];
            float a_val2 = b[i + 2] * c[i + 2] * d[i + 2];
            float a_val3 = b[i + 3] * c[i + 3] * d[i + 3];
            
            // Store computed a[i] values
            a[i] = a_val0;
            a[i + 1] = a_val1;
            a[i + 2] = a_val2;
            a[i + 3] = a_val3;
            
            // Compute b[i] = a[i] * original_a_next * d[i]
            b[i] = a_val0 * a_next0 * d[i];
            b[i + 1] = a_val1 * a_next1 * d[i + 1];
            b[i + 2] = a_val2 * a_next2 * d[i + 2];
            b[i + 3] = a_val3 * a_next3 * d[i + 3];
        }
        
        // Scalar cleanup for remaining elements
        for (; i < inner_len; i++) {
            float a_next = a[i + 1];
            a[i] = b[i] * c[i] * d[i];
            b[i] = a[i] * a_next * d[i];
        }
    }
}
