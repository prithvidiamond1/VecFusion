#include <stddef.h>

void vectorized_s341(int iterations, int LEN_1D, float* a, float* b) {
    for (int nl = 0; nl < iterations; nl++) {
        int write_idx = 0;
        int i = 0;
        
        // Process elements in chunks of 4 for potential vectorization
        for (; i + 3 < LEN_1D; i += 4) {
            float temp0 = b[i];
            float temp1 = b[i + 1];
            float temp2 = b[i + 2];
            float temp3 = b[i + 3];
            
            int mask0 = temp0 > 0.0f;
            int mask1 = temp1 > 0.0f;
            int mask2 = temp2 > 0.0f;
            int mask3 = temp3 > 0.0f;
            
            // Store all values first
            a[write_idx] = temp0;
            write_idx += mask0;
            a[write_idx] = temp1;
            write_idx += mask1;
            a[write_idx] = temp2;
            write_idx += mask2;
            a[write_idx] = temp3;
            write_idx += mask3;
        }
        
        // Scalar cleanup for remaining elements
        for (; i < LEN_1D; i++) {
            float temp = b[i];
            int mask = temp > 0.0f;
            a[write_idx] = temp;
            write_idx += mask;
        }
    }
}
