#include <stddef.h>

void vectorized_s112(float *a, float *b, int iterations, int LEN_1D) {
    int total_outer = 3 * iterations;
    
    for (int nl = 0; nl < total_outer; nl++) {
        // Vectorized reverse processing
        int i = LEN_1D - 2;
        
        // Process vector chunks in reverse order
        for (; i >= 3; i -= 4) {
            // Load current elements
            float a0 = a[i];
            float a1 = a[i-1];
            float a2 = a[i-2];
            float a3 = a[i-3];
            
            float b0 = b[i];
            float b1 = b[i-1];
            float b2 = b[i-2];
            float b3 = b[i-3];
            
            // Compute temp values
            float temp0 = a0 + b0;
            float temp1 = a1 + b1;
            float temp2 = a2 + b2;
            float temp3 = a3 + b3;
            
            // Store to next positions (i+1, i, i-1, i-2)
            a[i+1] = temp0;
            a[i] = temp1;
            a[i-1] = temp2;
            a[i-2] = temp3;
        }
        
        // Scalar cleanup for remaining elements
        for (; i >= 0; i--) {
            float temp = a[i] + b[i];
            a[i + 1] = temp;
        }
    }
}
