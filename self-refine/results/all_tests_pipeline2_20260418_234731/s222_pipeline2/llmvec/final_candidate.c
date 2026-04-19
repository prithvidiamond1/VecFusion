#include <stddef.h>

void vectorized_s222(int iterations, int LEN_1D, float* a, float* b, float* c, float* e) {
    for (int nl = 0; nl < iterations/2; nl++) {
        // Handle recurrence for e with scalar start
        int i = 1;
        // Process first element to establish dependency chain
        if (i < LEN_1D) {
            e[i] = e[i - 1] * e[i - 1];
            i++;
        }
        
        // Vectorized recurrence loop for remaining elements
        for (; i + 3 < LEN_1D; i += 4) {
            // Load the four consecutive values that will be squared
            // These are the values computed in previous iterations
            float prev0 = e[i - 1];  // Already computed
            float prev1 = prev0 * prev0;  // Compute e[i]
            float prev2 = prev1 * prev1;  // Compute e[i+1]
            float prev3 = prev2 * prev2;  // Compute e[i+2]
            
            // Store the computed values
            e[i]     = prev1;
            e[i + 1] = prev2;
            e[i + 2] = prev3;
            e[i + 3] = prev3 * prev3;  // Compute and store e[i+3]
        }
        
        // Scalar tail for the recurrence
        for (; i < LEN_1D; i++) {
            e[i] = e[i - 1] * e[i - 1];
        }

        // Vectorized independent a updates (net zero effect)
        i = 1;
        for (; i + 3 < LEN_1D; i += 4) {
            float b0 = b[i];
            float b1 = b[i + 1];
            float b2 = b[i + 2];
            float b3 = b[i + 3];

            float c0 = c[i];
            float c1 = c[i + 1];
            float c2 = c[i + 2];
            float c3 = c[i + 3];

            float temp0 = b0 * c0;
            float temp1 = b1 * c1;
            float temp2 = b2 * c2;
            float temp3 = b3 * c3;

            a[i]     += temp0;
            a[i]     -= temp0;
            a[i + 1] += temp1;
            a[i + 1] -= temp1;
            a[i + 2] += temp2;
            a[i + 2] -= temp2;
            a[i + 3] += temp3;
            a[i + 3] -= temp3;
        }
        // Scalar tail for a updates
        for (; i < LEN_1D; i++) {
            float temp = b[i] * c[i];
            a[i] += temp;
            a[i] -= temp;
        }
    }
}
