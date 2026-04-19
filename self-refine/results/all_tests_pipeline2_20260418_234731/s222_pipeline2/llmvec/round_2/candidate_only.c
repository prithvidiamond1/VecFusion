#include <stddef.h>

void vectorized_s222(int iterations, int LEN_1D, float* a, float* b, float* c, float* e) {
    for (int nl = 0; nl < iterations/2; nl++) {
        // Vectorized recurrence loop for e
        int i = 1;
        for (; i + 3 < LEN_1D; i += 4) {
            // Load the four consecutive original values starting at e[i-1]
            float prev0 = e[i - 1];
            float prev1 = e[i];
            float prev2 = e[i + 1];
            float prev3 = e[i + 2];
            
            // Square them and store starting at e[i]
            e[i]     = prev0 * prev0;
            e[i + 1] = prev1 * prev1;
            e[i + 2] = prev2 * prev2;
            e[i + 3] = prev3 * prev3;
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
