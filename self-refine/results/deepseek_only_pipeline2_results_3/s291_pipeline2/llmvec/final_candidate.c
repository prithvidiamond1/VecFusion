#include <stddef.h>

void vectorized_s291(int iterations, int LEN_1D, float* a, float* b) {
    if (LEN_1D <= 0 || iterations <= 0) return;
    
    int outer_iterations = 2 * iterations;
    
    for (int nl = 0; nl < outer_iterations; nl++) {
        // Handle first element separately
        a[0] = (b[0] + b[LEN_1D-1]) * 0.5f;
        
        // Vectorized main loop
        int i = 1;
        
        // Process elements in chunks of 4 where possible
        for (; i + 3 < LEN_1D; i += 4) {
            // Load b[i] and b[i-1] for 4 consecutive elements
            float b0 = b[i];
            float b1 = b[i+1];
            float b2 = b[i+2];
            float b3 = b[i+3];
            
            float b_prev0 = b[i-1];
            float b_prev1 = b[i];
            float b_prev2 = b[i+1];
            float b_prev3 = b[i+2];
            
            // Compute (b[i] + b[i-1]) * 0.5f
            a[i]   = (b0 + b_prev0) * 0.5f;
            a[i+1] = (b1 + b_prev1) * 0.5f;
            a[i+2] = (b2 + b_prev2) * 0.5f;
            a[i+3] = (b3 + b_prev3) * 0.5f;
        }
        
        // Scalar cleanup for remaining elements
        for (; i < LEN_1D; i++) {
            a[i] = (b[i] + b[i-1]) * 0.5f;
        }
    }
}
