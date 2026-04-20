#include <stddef.h>

void vectorized_s116(int iterations, int LEN_1D, float* a) {
    if (LEN_1D < 6) return;
    
    int outer_iterations = iterations * 10;
    int vector_limit = LEN_1D - 6;
    
    for (int nl = 0; nl < outer_iterations; nl++) {
        int i = 0;
        
        // Vector-friendly main loop: process 5 outputs per iteration using 6 original values
        for (; i <= vector_limit; i += 5) {
            // Load 6 consecutive original values
            float orig_i = a[i];
            float orig_i1 = a[i + 1];
            float orig_i2 = a[i + 2];
            float orig_i3 = a[i + 3];
            float orig_i4 = a[i + 4];
            float orig_i5 = a[i + 5];
            
            // Compute 5 outputs using only these original values
            a[i] = orig_i1 * orig_i;
            a[i + 1] = orig_i2 * orig_i1;
            a[i + 2] = orig_i3 * orig_i2;
            a[i + 3] = orig_i4 * orig_i3;
            a[i + 4] = orig_i5 * orig_i4;
        }
        
        // Scalar cleanup for remaining elements (if any)
        int scalar_limit = LEN_1D - 5;
        for (; i < scalar_limit; i++) {
            float orig_i = a[i];
            float orig_i1 = a[i + 1];
            a[i] = orig_i1 * orig_i;
        }
    }
}
