#include <stddef.h>

void vectorized_s242(int iterations, int LEN_1D, float s1, float s2, float* a, float* b, float* c, float* d) {
    float sum_scalar = s1 + s2;
    int outer_loop_limit = iterations / 5;
    
    for (int nl = 0; nl < outer_loop_limit; nl++) {
        float prev_a = a[0];
        int i = 1;
        
        // Vectorized main loop
        for (; i + 3 < LEN_1D; i += 4) {
            // Load b[i..i+3], c[i..i+3], d[i..i+3]
            float b0 = b[i], b1 = b[i+1], b2 = b[i+2], b3 = b[i+3];
            float c0 = c[i], c1 = c[i+1], c2 = c[i+2], c3 = c[i+3];
            float d0 = d[i], d1 = d[i+1], d2 = d[i+2], d3 = d[i+3];
            
            // Compute temp values
            float temp0 = prev_a + sum_scalar + b0 + c0 + d0;
            float temp1 = temp0 + sum_scalar + b1 + c1 + d1;
            float temp2 = temp1 + sum_scalar + b2 + c2 + d2;
            float temp3 = temp2 + sum_scalar + b3 + c3 + d3;
            
            // Store results
            a[i] = temp0;
            a[i+1] = temp1;
            a[i+2] = temp2;
            a[i+3] = temp3;
            
            // Update prev_a for next iteration
            prev_a = temp3;
        }
        
        // Scalar cleanup for remaining elements
        for (; i < LEN_1D; i++) {
            float temp = prev_a + sum_scalar + b[i] + c[i] + d[i];
            a[i] = temp;
            prev_a = temp;
        }
    }
}
